//
//  op.cpp
//  pulsar
//
//  Created by Victor Grishchenko on 02/09/2019.
//  Copyright © 2019 gritzko. All rights reserved.
//

#include "op.hpp"

#include "ron-streams.hpp"
#include <iostream>

namespace ron {

using nonstd::span;

PROC Op::Parse(Op &op, const std::string &ront) {
    static thread_local RONtStream reader{};
    if (!reader.IsOpen()) {
        reader.Create();
    } else {
        reader.Reset();
    }
    CALL(reader.FeedString(ront));
    CALL(reader.FeedChar(';'));
    CALL(reader.DrainOp(op));
    DONE;
}

inline bool AreEqualRanges(span32 a, span32 b) {
    if (a.size() != b.size()) {
        return false;
    }
    for (Index i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

void Tuple::Allocated::Allocate(const Inlined &inlined) {
    auto bits = (Word *)malloc(FIRST_ALLOC_WORD_SIZE << 3U);
    bits[0] = inlined.desc_.Coded();
    bits[1] = inlined.bits_[0];
    bits[2] = inlined.bits_[1];
    // start overwriting
    size_ = inlined.desc_.RONcPayloadWordSize() + 1;
    lb_ndx_ = 0;
    padding_ = MAX_INDEX;  // sabotage the inlined box desc
    bits_ = bits;
    cap_ = FIRST_ALLOC_WORD_SIZE;
}

PROC Tuple::Allocated::Expand(Index min_word_size) {
    if (cap_ >= min_word_size) return OK;
    auto new_word_size = cap_;
    while (new_word_size < min_word_size) new_word_size <<= 1;
    size_t new_byte_size = size_t(new_word_size) << 3U;
    bits_ = (Word *)realloc(bits_, new_byte_size);
    cap_ = new_word_size;
    return bits_ ? OK : OUTOMEMRY.Ext(BASE_T);
}

PROC RONvStream::FeedTuple(const Tuple &tuple) {
    auto r = tuple.ReadBoxes();
    Tuple::Box box{};
    while (OK == r.ReadBox(box)) CALL(FeedBox(box));
    DONE;
}

PROC RONvStream::DrainTuple(Tuple &tuple) {
    Tuple::BoxDesc box{};
    tuple.Clear();
    do {
        CALL(DrainBox(box, tuple));
    } while (!box.IsLast());
    DONE;
}

PROC Tuple::GetUuid(Uuid &value) const {
    BoxReader reader{*this};
    Box box;
    CALL(reader.ReadBox(box));
    if (box.Type() != ID || box.Size() < 1) FAIL(BADRECORD.Ext(BASE_O));
    value = box.UuidAt(0);
    DONE;
}

PROC Tuple::GetSingularString(Codepoint &value) const {
    BoxReader reader{*this};
    Box box;
    CALL(reader.ReadBox(box));
    if (box.Type() != STRING || box.Size() != 1) FAIL(BADRECORD.Ext(BASE_O));
    value = box.CodepointAt(0);
    DONE;
}

PROC Tuple::GetFloat(Float &value) const {
    BoxReader reader{*this};
    Box box;
    CALL(reader.ReadBox(box));
    if (box.Type() != FLOAT || box.Size() < 1) FAIL(BADRECORD.Ext(BASE_O));
    value = box.FloatAt(0);
    DONE;
}

PROC Tuple::GetInteger(Integer &value) const {
    BoxReader reader{*this};
    Box box;
    CALL(reader.ReadBox(box));
    if (box.Type() != INT || box.Size() < 1) FAIL(BADRECORD.Ext(BASE_O));
    value = box.IntegerAt(0);
    DONE;
}

PROC Tuple::GetUuidUuid(Uuid &id, Uuid &value) const {
    BoxReader reader{*this};
    Box box;
    CALL(reader.ReadBox(box));
    NEED(box.Type() == ID && box.Size() >= 2, BADRECORD.Ext(BASE_O));
    id = box.UuidAt(0);
    value = box.UuidAt(1);
    DONE;
}

PROC Tuple::GetUuidInteger(Uuid &id, Integer &value) const {
    BoxReader reader{*this};
    Box id_box, value_box;
    CALL(reader.ReadBox(id_box));
    CALL(reader.ReadBox(value_box));
    if (id_box.Type() != ID || id_box.Size() != 1) FAIL(BADRECORD.Ext(BASE_O));
    if (value_box.Type() != INT || value_box.Size() != 1)
        FAIL(BADRECORD.Ext(BASE_O));
    id = id_box.UuidAt(0);
    value = value_box.IntegerAt(0);
    DONE;
}

PROC Tuple::GetUuidFloat(Uuid &id, Float &value) const {
    BoxReader reader{*this};
    Box id_box, value_box;
    CALL(reader.ReadBox(id_box));
    CALL(reader.ReadBox(value_box));
    if (id_box.Type() != ID || id_box.Size() != 1) FAIL(BADRECORD.Ext(BASE_O));
    if (value_box.Type() != FLOAT || value_box.Size() != 1)
        FAIL(BADRECORD.Ext(BASE_O));
    id = id_box.UuidAt(0);
    value = value_box.FloatAt(0);
    DONE;
}

PROC Tuple::GetUuidString(Uuid &id, span32c &value) const {
    BoxReader reader{*this};
    Box id_box, value_box;
    CALL(reader.ReadBox(id_box));
    CALL(reader.ReadBox(value_box));
    if (id_box.Type() != ID || id_box.Size() != 1) FAIL(BADRECORD.Ext(BASE_O));
    if (value_box.Type() != STRING) FAIL(BADRECORD.Ext(BASE_O));
    id = id_box.UuidAt(0);
    value = value_box.String();
    DONE;
}

PROC Tuple::GetStringString(span32c &key, span32c &value) const {
    BoxReader reader{*this};
    Box box_key, box_value;
    CALL(reader.ReadBox(box_key));
    NEED(box_key.Type() == STRING, BADRECORD.Ext(BASE_O));
    CALL(reader.ReadBox(box_value));
    NEED(box_value.Type() == STRING, BADRECORD.Ext(BASE_O));
    key = box_key.String();
    value = box_value.String();
    DONE;
}

std::string Tuple::Pattern() const {
    std::string ret{};
    auto r = ReadBoxes();
    while (!r.IsEmpty()) {
        Box box{};
        r.ReadBox(box);
        if (box.Type() == STRING) {
            ret.push_back(ATOM_PUNCT[STRING]);
        } else if (box.Type() == ID) {
            while (!box.IsEmpty()) {
                ret.push_back(UUID_PUNCT[box.UuidAt(0).Version()]);
                box = box.SubBox(1);
            }
        } else {
            ret.append(box.Size(), ATOM_PUNCT[box.Type()]);
        }
    }
    return ret;
}

PROC Tuple::Has(Uuid id) const {
    BoxReader reader{*this};
    while (!reader.IsEmpty()) {
        Box box;
        CALL(reader.ReadBox(box));
        if (box.Type() != ID) continue;
        for (auto i = 0; i < box.Size(); ++i)
            if (box.UuidAt(i) == id) DONE;
    }
    FAIL(NOTFOUND);
}


PROC Tuple::Parse(Tuple &into, const std::string &ront) {
    static thread_local RONtStream reader{};
    if (!reader.IsEmpty()) {
        reader.Reset();
    } else {
        reader.Create();
    }
    CALL(reader.FeedString(ront));
    CALL(reader.FeedChar(';'));
    Op op{};
    CALL(reader.DrainOp(op));
    std::swap(into, op.Values());
    DONE;
}


PROC Tuple::Box::Verify() const {
    NEED(Size() > 0 || Type() == ATOM::ID, BADRECORD.Ext(BASE_b));
    NEED(Descriptor().RONcPayloadWordSize() <= MAX_INDEX, BADSIZE.Ext(BASE_T));
    if (Type() == STRING) {
        for (Index i = 0; i < Size(); ++i)
            NEED(CodepointAt(i) <= MAX_UNICODE_CODEPOINT,
                 BADRECORD.Ext(BASE_b));
    } else if (Type() == FLOAT) {
        for (Index i = 0; i < Size(); ++i) {
            auto f = FloatAt(i);
            NEED(!isnan(f) && !isinf(f), BADRECORD.Ext(BASE_b));
        }
    } else if (Type() == ID) {
        for (Index i = 0; i < Size(); ++i) CALL(UuidAt(i).Verify());
    }  // INTs are all OK
    DONE;
}

PROC Tuple::Verify() const {
    auto br = ReadBoxes();
    Box box{};
    PROC s = br.ReadBox(box);
    CALL(box.Verify());
    ATOM pre = box.Type();
    WHILE(boxx, br.ReadBox(box)) {
        CALL(box.Verify());
        NEED(box.Type() != pre || box.Type() == STRING, BADRECORDB);
        pre = box.Type();
    }
    // FIXME last?
    NEED_EOF(boxx);
    DONE;
}

std::string Tuple::Debug() const {
    auto data = Data();
    std::string ret;
    char hex[33];
    hex[32] = 0;
    for (auto w : data) {
        sprintf(hex, "[%x %x] ", Halves::Of(w).ls_half, Halves::Of(w).ms_half);
        ret.append(hex);
    }
    return ret;
}

void Report(Status status) {
    auto errcode = Word2String(status._s);
    auto message = ErrorMessage(status);
    std::string msg{};
    auto span = as_span32c(message);
    SaveUtf8(msg, span);
    std::cerr << errcode << '\t' << msg << '\n';
}

}  // namespace ron
