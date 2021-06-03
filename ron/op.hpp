//
//  op.hpp
//  pulsar
//
//  Created by Victor Grishchenko on 02/09/2019.
//  Copyright © 2019 gritzko. All rights reserved.
//

#ifndef RON_OP_HPP
#define RON_OP_HPP

#include <math.h>

#include <cinttypes>
#include <cstdlib>
#include <vector>

#include "utf8.hpp"
#include "uuid.hpp"

namespace ron {

using Words = std::vector<Word>;

/** RONc/RONv/RONvp box descriptor; A box can host a collection of
 * INT/FLOAT/UUID atoms or a single STRING atom. Normally part of a pallet. */
struct BoxDescriptor {
    /** popular descriptor values */
    enum {
        EMPTY = 0,
        ID0LAST = 0,  // empty op in an even chain
        ID0MORE = 1,  // RONv even chain (can't skip the spec)
        FACT_TERM = 2,
        INT0LAST = 2,  // FACT frame terminator
        INT0MORE = 3,  // RONp even chain (can't skip the spec)
        CLAIM_TERM = 4,
        STRING0LAST = 4,  // CLAIM frame terminator
        STRING0MORE = 5,  // even chain, spec skipped, STRING
        QUERY_TERM = 6,
        FLOAT0LAST = 6,    // QUERY frame terminator
        FLOAT0MORE = 7,    // invalid
        ID1LAST = 8,       // even chain, spec skipped, the payload is an UUID
        ID1MORE = 9,       // RONv even chain spec
        INT1LAST = 10,     // tight chain, the payload is an INT
        INT1MORE = 11,     // RONp op, single-entry specifier box
        STRING1LAST = 12,  // tight chain, the payload is a singular STRING
        STRING1MORE = 13,  // even chain, spec skipped, first atom is a STRING
        FLOAT1LAST = 14,   // tight chain, the payload is a single FLOAT
        FLOAT1MORE = 15,   // even chain, first atom is a FLOAT
        ID2LAST = 16,      // even chain, the payload is two UUIDs
        ID2MORE = 17,      // RONv full spec (no chain)
        INT2LAST = 18,     // even chain, the payload is two INTs
        INT2MORE = 19,     // RONp spec of two ints
        FLOAT2LAST = 22,
    };
    static constexpr Word MAX_BOX_DESC = (Word(MAX_INDEX) << 3U) | 7U;

    Word coded_;

    BoxDescriptor() = default;
    explicit BoxDescriptor(Word coded) : coded_{coded} {}
    BoxDescriptor(ATOM atype, Index size, bool is_last = true)
        : coded_{(1UL & !is_last) | (atype << 1UL) | (Word(size) << 3UL)} {}

    /** the number of entries in the box (INTs, FLOATs, UUIDs, Codepoints) */
    Index Size() const { return coded_ >> 3; }
    bool IsEmpty() const { return coded_ < (1 << 3); }
    /** atom type (a box of INTs, FLOATs or UUIDs, or a single STRING) */
    ATOM Type() const { return ATOM(3 & (coded_ >> 1)); }
    /** whether the box is the last in the pallet (if serialized, bit 0 = last)
     */
    bool IsLast() const { return !(coded_ & 1); }
    /** Represented as a single (var)int */
    Word Coded() const { return coded_; }
    bool Is(Word coded) const { return coded == coded_; }
    bool IsValid() const { return IsValid(coded_); }
    /** singular or empty in a solid even chain, or a frame terminator */
    bool IsSimple() const { return coded_ == (coded_ & FLOAT1LAST); }
    /** Whether the descriptor can be seen as a RONp/RONv spec box desc */
    bool IsRONvpAmbiguous() const { return !IsLast() && Type() <= INT; }

    static bool IsValid(Word coded) { return !(coded & ~MAX_BOX_DESC); }
    /** RONc 64 bit word size of the payload */
    Index RONcPayloadWordSize() const {
        Index ret = Size();
        if (Type() == STRING) {
            ret = (ret & 1U) + (ret >> 1U);
        } else if (Type() == ID) {
            ret <<= 1U;
        }
        return ret;
    }
    Index RONcByteSize() const { return RONcPayloadWordSize() << 3U; }
    bool IsSingleWord() const {
        if ((coded_ & 9U) != 8U) return false;
        return Type() != ID;
    }

    void Set(Word coded) { coded_ = coded; }
    void Inc() { coded_ += (1U << 3U); }
    void Dec() { coded_ -= (1U << 3U); }
    void Inc(Index size) { coded_ += (size << 3U); }
    void Trim(Index size) {
        assert(size <= Size());
        coded_ = (coded_ & 7) | (Word{size} << 3);
    }
    void SetNotLast() { coded_ |= 1U; }
    void SetLast() { coded_ &= ~1U; }

    bool operator==(Word coded) const { return coded_ == coded; }
    /** RONp tight op descriptor: last box, size replaced by PRED */
    Index RONpTightDesc(Index pred) const {
        return (coded_ & 0x6U) | (pred << 3U);
    }
    bool operator==(const BoxDescriptor &b) const {
        return (coded_ ^ b.coded_) <= 1;
    }
    bool operator!=(const BoxDescriptor &b) const { return !(*this == b); }
    bool operator<(BoxDescriptor b) const {
        if (Type() == b.Type()) return Size() < b.Size();
        return Type() < b.Type();
    }
    bool operator<=(BoxDescriptor b) const {
        if (Type() == b.Type()) return Size() <= b.Size();
        return Type() <= b.Type();
    }
};

/** A RONc tuple of RON atoms (INT, UUID, FLOAT, STRING).
 *  Uses inlining to avoid allocations for shorter tuples.
 *  Writes are strictly append-only.
 *  Little-endian CPUs only.
 */
class Tuple {
   public:
    using BoxDesc = BoxDescriptor;
    struct Box;

   private:
    /** Tuple - the inlined version (max 3 words). May host 2 INTs or 1 UUID
     *  or a STRING of up to 4 codepoints. The inlined version is very valuable
     *  for containers, as it saves on allocation and indirection of small
     *  tuples. */
    struct Inlined {  // also 3 words
        BoxDesc desc_;
        Word bits_[2];

        Inlined() = default;

        BoxDesc LastBoxDesc() const { return desc_; }
        BoxDesc &LastBoxDesc() { return desc_; }
        span64 Data() const {
            return span64{(Word *)this, desc_.RONcPayloadWordSize() + 1};
        }
        span8c Data8() const {
            auto bytes = (desc_.RONcPayloadWordSize() + 1) << 3U;
            return span8c{(uint8_t *)this, bytes};
        }
        BoxDesc FirstBoxDesc() const { return desc_; }
        Box FirstBox() const {
            return Box{desc_,
                       span64{(Word *)bits_, desc_.RONcPayloadWordSize()}};
        }
        Box LastBox() const { return FirstBox(); }
        bool IsSingleAtom() const {
            return desc_.Type() == STRING || desc_.Size() == 1;
        }
        span32 Raw32() { return span32{(uint32_t *)this, 6}; }
        PROC PushInteger(Integer value) {
            if (desc_.Is(BoxDesc::EMPTY)) {
                desc_.Set(BoxDesc::INT1LAST);
                bits_[0] = (Word &)value;
            } else if (desc_.Is(BoxDesc::INT1LAST)) {
                desc_.Set(BoxDesc::INT2LAST);
                bits_[1] = (Word &)value;
            } else {
                FAIL(BADSIZE.Ext(BASE_I));
            }
            DONE;
        }
        PROC PushFloat(Float value) {
            if (desc_.Is(BoxDesc::EMPTY)) {
                desc_.Set(BoxDesc::FLOAT1LAST);
                bits_[0] = (Word &)value;
            } else if (desc_.Is(BoxDesc::FLOAT1LAST)) {
                desc_.Set(BoxDesc::FLOAT2LAST);
                bits_[1] = (Word &)value;
            } else {
                FAIL(BADSIZE.Ext(BASE_I));
            }
            DONE;
        }
        PROC PushUuid(Uuid id) {
            NEED(desc_.Is(BoxDesc::EMPTY), BADSIZE.Ext(BASE_I));
            desc_.coded_ = BoxDesc::ID1LAST;
            bits_[0] = id.first;
            bits_[1] = id.second;
            DONE;
        }
        PROC PushNewBox(BoxDesc desc) {
            NEED(desc_.Is(BoxDesc::EMPTY), BADSIZE.Ext(BASE_I));
            NEED(desc.RONcPayloadWordSize() <= 2, BADSIZE.Ext(BASE_I));
            desc_ = desc;
            DONE;
        }
        PROC PushCodepoint(Codepoint cp) {
            if (desc_ == BoxDesc::EMPTY) desc_.Set(BoxDesc::STRING0LAST);
            if (desc_.Type() == STRING && desc_.Size() < 4) {
                ((uint32_t *)bits_)[desc_.Size()] = cp;
                desc_.Inc();
                DONE;
            }
            FAIL(BADSIZE.Ext(BASE_I));
        }
        PROC PushString(span32c value) {
            if (!LastBoxDesc().Is(BoxDesc::EMPTY) || value.size() > 4)
                FAIL(BADSIZE.Ext(BASE_I));
            desc_ = BoxDesc{STRING, (Index)value.size()};
            bits_[0] = bits_[1] = 0;
            if (value.size()) memcpy(bits_, value.data(), value.size() << 2U);
            DONE;
        }
        void Clear() {
            desc_.coded_ = BoxDesc::EMPTY;
            bits_[0] = 0;
            bits_[1] = 0;
        }
        void Free() { Clear(); }
        PROC PushNewBox(span64 &values, BoxDesc desc) {
            NEED(desc_ == BoxDesc::EMPTY, BADSIZE.Ext(BASE_I));
            auto s = desc.RONcPayloadWordSize();
            NEED(s <= 2, BADSIZE.Ext(BASE_I));
            desc_ = desc;
            desc_.SetLast();
            values = span64{&bits_[0], s};
            DONE;
        }
        void TrimLastBox(Index new_size) { LastBoxDesc().Trim(new_size); }
    };

    /** Tuple - the allocated version; pointer and things. */
    struct Allocated {  // 3 words
        Index lb_ndx_;
        Index padding_;
        /** the allocated memory */
        Word *bits_;
        /** the word-size of the tuple */
        Index size_;
        /** the word-size of the allocated memory */
        Index cap_;

        BoxDesc LastBoxDesc() const { return (BoxDesc &)(bits_[lb_ndx_]); }
        BoxDesc &LastBoxDesc() { return (BoxDesc &)(bits_[lb_ndx_]); }
        BoxDesc FirstBoxDesc() const { return BoxDesc{bits_[0]}; }
        Box FirstBox() const {
            auto fbd = FirstBoxDesc();
            return Box{
                fbd, span64{bits_ + 1, bits_ + 1 + fbd.RONcPayloadWordSize()}};
        }
        Box LastBox() const {
            auto l1st = lb_ndx_ + 1;
            return Box{LastBoxDesc(), span64{bits_ + l1st, size_ - l1st}};
        }
        bool IsSingleAtom() const {
            if (lb_ndx_) return false;
            if (LastBoxDesc().Type() == STRING) return true;
            return LastBoxDesc().Size() == 1;
        }
        Index RONcWordSize() const { return size_; }
        /** RONc frame size limit is 2^30 bytes, ~128M words */
        PROC Expand(Index min_word_size);
        void Allocate(const Inlined &inlined);
        Index WordsAvailable() const { return cap_ - size_; }
        span32 Raw32() { return span32{(uint32_t *)bits_, cap_ << 1}; }
        /** warning: forgets the pointer */
        void Zero() {
            memset(this, 0, sizeof(Allocated));  // inlined, empty
        }
        void Free() {
            free(bits_);
            Zero();
        }
        PROC PushNewBox(span64 &values, BoxDesc desc) {
            auto ws = desc.RONcPayloadWordSize();
            if (WordsAvailable() < ws + 1) CALL(Expand(size_ + ws + 1));
            CALL(PushNewBox(desc));
            values = span64{bits_ + size_, ws};
            size_ += ws;
            DONE;
        }
        PROC PushWord(Word w) {
            if (cap_ == size_) Expand(cap_ << 1U);
            bits_[size_++] = w;
            DONE;
        }
        PROC PushWords(span64c words) {
            auto need_size = size_ + words.size();
            if (need_size > cap_) Expand(need_size);
            memcpy(bits_ + size_, words.data(), words.size() << 3U);
            size_ = need_size;
            DONE;
        }
        PROC PushNewBox(BoxDesc desc) {
            desc.SetLast();
            if (LastBoxDesc() == BoxDesc::EMPTY) {
                LastBoxDesc() = desc;
            } else {
                NEED(desc.Type() == STRING ||
                         LastBoxDesc().Type() != desc.Type(),
                     BADRONBOXJ);
                LastBoxDesc().SetNotLast();
                lb_ndx_ = size_;
                PushWord(desc.Coded());
            }
            DONE;
        }
        void Dup() {
            auto bits = (Word *)malloc(cap_ << 3U);
            memcpy(bits, bits_, size_ << 3U);
            bits_ = bits;
        }
        span64 Data() const { return span64{bits_, size_}; }
        span8c Data8() const { return span8c{(uint8_t*)bits_, size_<<3}; }
        PROC PushInteger(Integer value) {
            if (LastBoxDesc().Type() != INT) {
                PushNewBox(BoxDesc{BoxDesc::INT1LAST});
            } else {
                LastBoxDesc().Inc();
            }
            PushWord((Word &)value);  // FIXME ensure 2-complement in bits.hpp
            DONE;
        }
        PROC PushFloat(Float value) {
            if (LastBoxDesc().Type() != FLOAT) {
                PushNewBox(BoxDesc{FLOAT, 1});
            } else {
                LastBoxDesc().Inc();
            }
            return PushWord((Word &)value);
        }
        PROC PushUuid(Uuid id) {
            if (LastBoxDesc().Type() != ID) {
                PushNewBox(BoxDesc{BoxDesc::ID1LAST});
            } else {
                LastBoxDesc().Inc();
            }
            PushWord(id.first);  // note the order
            PushWord(id.second);
            DONE;
        }
        PROC PushCodepoint(Codepoint cp) {
            auto &desc = LastBoxDesc();
            if (desc.Type() != STRING) PushNewBox(BoxDesc{STRING, 0});
            if (LastBoxDesc().Size() & 1U) {
                Word *wbeg = bits_ + lb_ndx_ + 1;
                ((Codepoint *)wbeg)[desc.Size()] = cp;
            } else {
                PushWord(cp);
            }
            LastBoxDesc().Inc();
            DONE;
        }
        PROC PushString(span32c value) {
            PushNewBox(BoxDesc{STRING, (Index)value.size()});
            if (value.empty()) return OK;
            auto full_words = value.size() >> 1U;
            PushWords(span64{(Word *)value.data(), full_words});
            if (value.size() & 1U) PushWord(value.back());
            DONE;
        }
        /** Pushes the entire box into the tuple (may merge it with the
         *  last box to keep the tuple normalized) */
        PROC PushBox(Box box) {
            if (box.IsEmpty() && box.Type() != STRING) DONE;
            if (box.Type() == STRING || LastBoxDesc().Type() != box.Type()) {
                PushNewBox(box.Descriptor());
            } else {
                LastBoxDesc().Inc(box.Size());
            }
            return PushWords(box.Data());
        }
        PROC PushTuple(const Tuple &values) {
            BoxReader reader = values.ReadBoxes();
            Box box;
            PROC status = reader.ReadBox(box);
            if (box.Type() == LastBoxDesc().Type() && box.Type() != STRING) {
                auto merged_size = box.Size() + LastBoxDesc().Size();
                LastBoxDesc() = BoxDesc{box.Type(), merged_size};
                PushWords(box.Data());
                status = reader.ReadBox(box);
            }
            while (status == OK) {
                PushBox(box);
                status = reader.ReadBox(box);
            }
            return status == ENDOFRAME ? OK : status;
        }
        void TrimLastBox(Index new_size) {
            auto w = LastBoxDesc().RONcPayloadWordSize();
            LastBoxDesc().Trim(new_size);
            size_ -= w - LastBoxDesc().RONcPayloadWordSize();
        }
        void Clear() {
            size_ = 1;
            bits_[0] = BoxDesc::EMPTY;
            lb_ndx_ = 0;
        }
    };

    union {
        Inlined inlined_;
        Allocated allocated_;
    };

    static constexpr Index FIRST_ALLOC_WORD_SIZE = 8;
    friend class RONtStream;

   protected:
    bool IsAllocated() const { return !inlined_.desc_.IsValid(); }
    bool IsInlined() const { return !IsAllocated(); }
    bool IsEmptyInlined() const {
        return IsInlined() && inlined_.desc_ == BoxDesc::EMPTY;
    }
    void Free() { return IsInlined() ? inlined_.Free() : allocated_.Free(); }

    void Allocate() {
        assert(IsInlined());
        allocated_.Allocate(inlined_);
    }

    PROC PushNewBox(BoxDesc desc) {
        if (IsInlined()) {
            if (inlined_.PushNewBox(desc).Ok()) return OK;
            Allocate();
        }
        return allocated_.PushNewBox(desc);
    }

    /** all the allocated space, as span32 */
    span32 Raw32() {
        return IsInlined() ? inlined_.Raw32() : allocated_.Raw32();
    }

   public:
    Tuple() : inlined_{} {}
    Tuple(const Tuple &b) : inlined_{b.inlined_} {
        if (b.IsAllocated()) allocated_.Dup();
    }
    Tuple(Tuple &&b) : inlined_{b.inlined_} {
        if (b.IsAllocated()) b.allocated_.Zero();
    }
    ~Tuple() {
        if (IsAllocated()) free(allocated_.bits_);
    }

    template <typename... Args>
    explicit Tuple(const Args... args) : Tuple{} {
        PushAtoms(args...);
    }

    explicit Tuple(Box box) : Tuple{} { PushBox(box); }

    Tuple &operator=(const Tuple &b) {
        if (IsAllocated()) allocated_.Free();
        memcpy((void *)this, (void *)&b, sizeof(Tuple));
        if (b.IsAllocated()) allocated_.Dup();
        return *this;
    }
    Tuple &operator=(Tuple &&b) noexcept {
        if (IsAllocated()) allocated_.Free();
        memcpy((void *)this, (void *)&b, sizeof(Tuple));
        if (b.IsAllocated()) memset((void *)&b, 0, sizeof(Tuple));
        return *this;
    }
    Tuple &operator=(const Allocated &b) {
        if (IsAllocated())
            allocated_.Free();  // TODO optimize this reallocation
        memcpy((void *)this, (void *)&b, sizeof(Tuple));
        allocated_.Dup();
        return *this;
    }

    BoxDescriptor FirstBoxDesc() const {
        return IsInlined() ? inlined_.FirstBoxDesc()
                           : allocated_.FirstBoxDesc();
    }
    BoxDescriptor LastBoxDesc() const {
        return IsInlined() ? inlined_.LastBoxDesc() : allocated_.LastBoxDesc();
    }
    /** Trims the last box without deallocating or erasing the data */
    void TrimLastBox(Index new_len) {
        IsInlined() ? inlined_.TrimLastBox(new_len)
                    : allocated_.TrimLastBox(new_len);
    }
    Box FirstBox() const {
        return IsInlined() ? inlined_.FirstBox() : allocated_.FirstBox();
    }
    Box LastBox() const {
        return IsInlined() ? inlined_.LastBox() : allocated_.LastBox();
    }

    bool IsSingleBox() const { return IsInlined() || allocated_.lb_ndx_ == 0; }

    bool IsString() const {
        return IsSingleBox() && LastBoxDesc().Type() == STRING;
    }

    bool IsSingular() const {
        if (!IsSingleBox()) return false;
        if (LastBoxDesc().Type() == ID) return LastBoxDesc().IsEmpty();
        return LastBoxDesc().Size() == 1;
    }

    /** has one box of one atom exactly, i.e. 1, 1.0, 'a', {one}; */
    bool IsSingleAtom() const {
        return IsInlined() ? inlined_.IsSingleAtom()
                           : allocated_.IsSingleAtom();
    }

    bool IsSingularString() const {
        return IsSingleBox() && LastBoxDesc() == BoxDesc::STRING1LAST;
    }

    bool IsSingularInt() const {
        return IsSingleBox() && LastBoxDesc() == BoxDesc::INT1LAST;
    }

    bool IsEmpty() const {
        return IsSingleBox() && LastBoxDesc() == BoxDesc::EMPTY;
    }

    bool IsEmptyString() const {
        return IsSingleBox() && LastBoxDesc() == BoxDesc::STRING0LAST;
    }

    bool IsOneID() const {
        return IsSingleBox() && LastBoxDesc() == BoxDesc::ID1LAST;
    }

    std::string Pattern() const;

    /** whether the tuple is RONt-spreadable at all (both STRING and IFU) */
    bool IsSpreadable() const {
        if (IsEmpty()) return true;
        if (!IsSingleAtom()) return false;
        if (LastBoxDesc().Type() != STRING) return true;
        return IsSingularString();
    }

    /** whether the tuple is RONt-spreadable with a singular tuple b */
    bool IsSpread(const Tuple &b) const {
        if (!IsSingleAtom()) return false;
        if (IsSingularString()) return b.IsSingularString();
        return Data() == b.Data();
    }

    bool IsEqual(const Tuple &b) const { return Data() == b.Data(); }

    /** The total tuple word-size. */
    Index RONcWordSize() const {
        if (IsAllocated()) return allocated_.RONcWordSize();
        return inlined_.desc_.RONcPayloadWordSize() + 1;
    }

    /** the byte size of the RONc tuple, including the padding */
    Index RONcByteSize() const { return RONcWordSize() << 3U; }

    bool operator==(const Tuple &b) const { return Data() == b.Data(); }
    bool operator!=(const Tuple &b) const { return Data() != b.Data(); }

    span64 Data() const {
        return IsInlined() ? inlined_.Data() : allocated_.Data();
    }
    span8c Data8() const {
        return IsInlined() ? inlined_.Data8() : allocated_.Data8();
    }

    Codepoint GetSingularString() const {
        assert(IsSingularString());
        return Halves(Data()[1]).ls_half;
    }

    Integer GetSingularInt() const {
        assert(IsSingularInt());
        return Integer(Data()[1]);
    }

    Float GetSingularFloat() const {
        return reinterpret_cast<Float &>(Data()[1]);
    }

    //  A P P E N D E R S

    PROC PushNewBox(span64 &values, BoxDesc desc) {
        if (IsInlined()) {
            if (inlined_.PushNewBox(values, desc).Ok()) return OK;
            Allocate();
        }
        return allocated_.PushNewBox(values, desc);
    }

    PROC PushInteger(Integer value) {
        if (IsInlined()) {
            if (inlined_.PushInteger(value).Ok()) return OK;
            Allocate();
        }
        return allocated_.PushInteger(value);
    }

    PROC PushFloat(Float value) {
        if (IsInlined()) {
            if (inlined_.PushFloat(value).Ok()) return OK;
            Allocate();
        }
        return allocated_.PushFloat(value);
    }

    PROC PushUuid(Uuid id) {
        if (IsInlined()) {
            if (inlined_.PushUuid(id).Ok()) return OK;
            Allocate();
        }
        return allocated_.PushUuid(id);
    }

    PROC PushNewBox(ATOM type) { return PushNewBox(BoxDesc{type, 0}); }

    PROC PushNewString() { return PushNewBox(ATOM::STRING); }

    PROC PushCodepoint(Codepoint cp) {
        //        if (cp>MAX_UNICODE_CODEPOINT) FAIL(BADUNICODE);
        //        if (cp>=0xD800 && cp<0xE000) FAIL(BADUNICODE);
        if (IsInlined()) {
            if (inlined_.PushCodepoint(cp).Ok()) return OK;
            Allocate();
        }
        return allocated_.PushCodepoint(cp);
    }

    template <typename... Atoms>
    PROC PushAtoms() {
        DONE;
    }

    template <typename... Atoms>
    PROC PushAtoms(Integer i, const Atoms... atoms) {
        CALL(PushInteger(i));
        return PushAtoms(atoms...);
    }

    template <typename... Atoms>
    PROC PushAtoms(Uuid uuid, const Atoms... atoms) {
        CALL(PushUuid(uuid));
        return PushAtoms(atoms...);
    }

    template <typename... Atoms>
    PROC PushAtoms(Float aFloat, const Atoms... atoms) {
        CALL(PushFloat(aFloat));
        return PushAtoms(atoms...);
    }

    template <typename... Atoms>
    PROC PushAtoms(std::string str, const Atoms... atoms) {
        CALL(PushUtf8String(str));
        return PushAtoms(atoms...);
    }

    template <typename... Atoms>
    PROC PushAtoms(span32c str, const Atoms... atoms) {
        CALL(PushString(str));
        return PushAtoms(atoms...);
    }

    void ReplaceCodepoint(Codepoint cp) {
        assert(LastBoxDesc().Type() == STRING && (LastBoxDesc().Size() & 1U));
        Data().back() = cp;
    }

    PROC ReplaceUuid(Uuid id) {
        NEED(LastBoxDesc().Type() == ID && !LastBoxDesc().IsEmpty(),
             NOTFOUND.Ext(BASE_t));
        auto lastidspan = Data().subspan(Data().size() - 2);
        auto idspan = id.AsWordSpan();
        lastidspan[0] = idspan[0];
        lastidspan[1] = idspan[1];
        DONE;
    }

    PROC PushString(span32c value) {
        assert(value.size() < MAX_INDEX);
        if (IsInlined()) {
            if (inlined_.PushString(value).Ok()) return OK;
            Allocate();
        }
        return allocated_.PushString(value);
    }

    PROC PushUtf8String(span8c value) {
        string32 parsed;
        CALL(ParseUtf8(parsed, value));
        PushString(as_span32c(parsed));
        DONE;
    }
    PROC PushUtf8String(const std::string &value) {
        return PushUtf8String(as_span8c(value));
    }

    PROC PushBox(Box box) {
        if (box.IsEmpty() && box.Type() != STRING) DONE;
        if (IsInlined()) Allocate();
        return allocated_.PushBox(box);
    }

    PROC PushTuple(const Tuple &values) {
        if (values.IsEmpty()) DONE;
        if (IsInlined()) Allocate();
        return allocated_.PushTuple(values);
    }

    void Clear() { return IsInlined() ? inlined_.Clear() : allocated_.Clear(); }

    void Swap(Tuple &b) { std::swap(inlined_, b.inlined_); }

    //  R E A D I N G

    /** A non-owning span of RON scalars (INT, FLOAT, UUID, CODEPOINT);
     *  may contain zero or more atoms, including a part of an atom (STRING) */
    class Box {
        BoxDesc desc_;
        Word *data_;

       public:
        Box() : desc_{}, data_{} {}
        Box(BoxDesc desc, span64 data) : desc_{desc}, data_{data.data()} {
            assert(WordSize() == data.size());
        }
        Box(BoxDesc desc, Word *data) : desc_{desc}, data_{data} {}
        Box(ATOM type, Index size, Word *data)
            : desc_{type, size}, data_{data} {}

        BoxDesc Descriptor() const { return desc_; }
        span64c Data() const { return span64{data_, WordSize()}; }
        ATOM Type() const { return Descriptor().Type(); }
        Index WordSize() const { return Descriptor().RONcPayloadWordSize(); }
        Index Size() const { return Descriptor().Size(); }
        Index Last() const { return Size() - 1; }
        bool IsLast() const { return desc_.IsLast(); }
        const Integer &IntegerAt(Index ndx) const {
            return reinterpret_cast<const Integer &>(data_[ndx]);
        }
        const Uuid &UuidAt(Index ndx) const {
            return reinterpret_cast<const ron::Uuid &>(data_[ndx << 1U]);
        }
        const double &FloatAt(Index ndx) const {
            return reinterpret_cast<const double &>(data_[ndx]);
        }
        const Codepoint &CodepointAt(Index ndx) const {
            return (reinterpret_cast<const ron::Codepoint *>(data_))[ndx];
        }
        span32c String() const {
            const auto *first = reinterpret_cast<const uint32_t *>(data_);
            return span32c{first, Size()};
        }
        bool IsEmpty() const { return WordSize() == 0; }
        /** An empty STRING is empty, but not void */
        bool IsVoid() const { return desc_ == 0; }
        /** A sub-box containing atoms. Can't split STRING boxes this way */
        Box SubBox(Index from, Index till = INVALID_INDEX) {
            if (till == INVALID_INDEX) till = Size();
            assert(till <= Size());
            assert(Type() != STRING);
            auto offset = Type() == ID ? from << 1U : from;
            return Box{BoxDesc{Type(), till - from}, data_ + offset};
        }

        Box operator[](Index ndx) {
            assert(ndx < Size());
            Index at = Type() == ID ? ndx << 1U : ndx;
            return Box{BoxDesc{Type(), 1}, data_ + at};
        }
        bool operator==(Integer i) const {
            return Type() == INT && Size() == 1 && IntegerAt(0) == i;
        }
        bool operator==(Float f) const {
            return Type() == FLOAT && Size() == 1 && FloatAt(0) == f;
        }
        bool operator==(Uuid id) const {
            return Type() == ID && Size() == 1 && UuidAt(0) == id;
        }
        bool operator==(span32c str) const {
            return Type() == STRING && Size() == str.size() && String() == str;
        }

        PROC Verify() const;

        bool operator==(const Box &b) const {
            return desc_ == b.desc_ && Data() == b.Data();
        }
        bool operator<(const Box &b) const {
            if (desc_ == b.desc_) return Data() < b.Data();
            return desc_ < b.desc_;
        }
        bool operator<=(const Box &b) const {
            if (desc_ == b.desc_) return Data() <= b.Data();
            return desc_ <= b.desc_;
        }
        bool operator==(const Tuple &t) const { return t == *this; }
        template <typename H>
        friend H AbslHashValue(H state, const Box &v) {
            state = H::combine(std::move(state), v.desc_.Coded());
            auto d = v.Data();
            for (auto w : d) state = H::combine(std::move(state), w);
            return state;
        }
    };

    /** Note that a write may invalidate iterators. */
    class BoxReader {
        span64 data_;

       public:
        explicit BoxReader(const Tuple &values) : data_{values.Data()} {}
        /** Warning: needs a correct word offset */
        BoxReader(const Tuple &values, Index offset) : BoxReader{values} {
            data_ = data_.subspan(offset);
        }

        inline bool IsEmpty() const { return data_.empty(); }
        inline span64c Data() const { return data_; }

        PROC ReadBox(Box &box) {
            if (IsEmpty()) return ENDOFRAME;
            BoxDesc d{(uint32_t)data_.front()};
            auto ws = d.RONcPayloadWordSize();
            NEED(ws < data_.size(), BADRECORDB);
            box = Box{d, data_.subspan(1, ws)};
            data_ = data_.subspan(ws + 1);
            DONE;
        }
    };

    BoxReader ReadBoxes() const { return BoxReader{*this}; }
    BoxReader ReadBoxes(Index offset) const { return BoxReader{*this, offset}; }

    /*PROC SaveSingular(VarIntWriter &writer) const {
        assert(IsSingular());
        auto t = FirstBoxDesc().Type();
        if (t == STRING) return SaveCodepoint(writer, GetSingularString());
        if (t == INT) return SaveInt(writer, GetSingularInt());
        if (t == FLOAT) return SaveFloat(writer, GetSingularFloat());
        FAIL(BADSTATE.Ext(BASE_T));
    }

    ** RONp singular boxes (empty or one codepoint, INT, FLOAT) *
    PROC LoadTight(VarIntReader &reader, ATOM type) {
        WordSpan rest;
        Clear();
        switch (type) {
            case ATOM::ID:
                DONE;
            case INT:
                CALL(PushNewBox(rest, BoxDesc{INT, 1}));
                return LoadInt(reader, (Integer &)rest[0]);
            case STRING:
                CALL(PushNewBox(rest, BoxDesc{STRING, 1}));
                rest.front() = 0;
                return LoadCodepoint(reader, (Codepoint &)rest[0]);
            case FLOAT:
                CALL(PushNewBox(rest, BoxDesc{FLOAT, 1}));
                return LoadFloat(reader, (Float &)rest[0]);
        }
        DONE;
    }*/

    PROC Hash(std::string &into) { FAIL(NOTIMPLTD); }

    bool IsOversized() const { return RONcByteSize() > MAX_FRAME_SIZE; }

    //  C O N V E N I E N C E

    /** Chained AddXyz methods, as a syntactic shortcut.
     *  Alternatives for this syntax are either variadic templates or
     *  stream operators. Those involve some excess smartness, like
     *  implicit conversions and tricky error handling. So no, KISS.  */
    Tuple &AddInt(Integer value) {
        PushInteger(value);
        return *this;
    }

    Tuple &AddFloat(Float value) {
        PushFloat(value);
        return *this;
    }

    Tuple &AddUtf8String(const std::string &value) {
        PushUtf8String(value);
        return *this;
    }

    Tuple AddString(const Codepoint *str) {
        PushString(as_span32c(str));
        return *this;
    }

    Tuple &AddUuid(Uuid id) {
        PushUuid(id);
        return *this;
    }

    Tuple &AddUuid(const std::string &id_str) {
        Uuid id{id_str};
        return AddUuid(id);
    }

    /** A convenience wrapper for BoxReader; reads the tuple in terms of RON
     * atoms. */
    class AtomReader {
        Box box_;
        BoxReader reader_;

        PROC Reload() {
            if (!box_.IsEmpty()) DONE;
            box_ = Box{};
            if (!reader_.IsEmpty()) CALL(reader_.ReadBox(box_));
            DONE;
        }

        friend class Tuple;

       public:
        AtomReader(const Tuple &orig) : box_{}, reader_{orig} {
            // in a valid tuple there is always at least one box
            if (!reader_.IsEmpty()) reader_.ReadBox(box_);
        }
        bool IsEmpty() const { return box_.IsVoid(); }
        bool IsLast() const {
            return reader_.IsEmpty() &&
                   (box_.Type() == STRING || box_.Size() == 1);
        }
        bool Has(ATOM type) const { return !IsEmpty() && box_.Type() == type; }
        ATOM Type() const { return IsEmpty() ? ID : box_.Type(); }
        bool HasUuid(UUID version) {
            return Has(ID) && box_.UuidAt(0).Version() == version;
        }
        bool Has(const Box &box) const {
            if (Type() != box.Type()) return false;
            if (box.Size() > box_.Size()) return false;
            return box_.Data().subspan(0, box.Size()) == box.Data();
        }
        PROC ReadInteger(Integer &value) {
            NEED(box_.Type() == INT, NOTFOUND.Ext(BASE_I));
            value = box_.IntegerAt(0);
            box_ = box_.SubBox(1);
            return Reload();
        }
        PROC ReadFloat(Float &value) {
            NEED(box_.Type() == FLOAT, NOTFOUND.Ext(BASE_F));
            value = box_.FloatAt(0);
            box_ = box_.SubBox(1);
            return Reload();
        }
        PROC ReadUuid(Uuid &value) {
            NEED(box_.Type() == ID && !box_.IsEmpty(), NOTFOUND.Ext(BASE_U));
            value = box_.UuidAt(0);
            box_ = box_.SubBox(1);
            return Reload();
        }
        PROC ReadString(string32 &value) {
            value.clear();
            NEED(box_.Type() == STRING, NOTFOUND0S);
            auto s = box_.String();
            value.insert(value.end(), s.begin(), s.end());
            box_ = Box{};
            return Reload();
        }
        PROC ReadUtf8String(std::string &value) {
            value.clear();
            NEED(box_.Type() == STRING, NOTFOUND.Ext(BASE_S));
            SaveUtf8(value, box_.String());
            box_ = Box{};
            return Reload();
        }
        PROC ReadUuidAsString(std::string& value) {
            Uuid tmp;
            CALL(ReadUuid(tmp));
            tmp.ToString(value);
            DONE;
        }
        PROC ReadAtom(Box &box) {
            NEED(!IsEmpty(), ENDOFRAME.Ext(BASE_B));
            if (box_.Type() == STRING) {
                box = box_;
                box_ = Box{};
            } else {
                box = box_.SubBox(0, 1);
                box_ = box_.SubBox(1);
            }
            return Reload();
        }
        PROC ReadBox(Box &box) {
            box = box_;
            box_ = Box{};
            return Reload();
        }
        PROC Next() {
            Box skip;
            return ReadAtom(skip);
        }
    };

    AtomReader ReadAtoms() const { return AtomReader{*this}; }

    PROC PushAtom(AtomReader &reader) {
        assert(!reader.IsEmpty());
        auto &b = reader.box_;
        if (reader.Type() == STRING) {
            PushBox(b);
            b = Box{};
        } else {
            auto sb = b.SubBox(0, 1);
            PushBox(sb);
            b = b.SubBox(1);
        }
        return reader.Reload();
    }

    PROC GetInteger(Integer &value) const;
    PROC GetFloat(Float &value) const;
    PROC GetString(span32c &value) const {
        BoxReader reader{*this};
        Box box;
        CALL(reader.ReadBox(box));
        if (box.Type() != STRING) FAIL(BADRECORD.Ext(BASE_O));
        value = box.String();
        DONE;
    }
    PROC GetString(string32 &value) const {
        span32c v;
        CALL(GetString(v));
        value.clear();
        value.reserve(v.size());
        value.insert(value.end(), v.begin(), v.end());
        DONE;
    }
    PROC GetUtf8String(std::string &utf8) const {
        span32c str{};
        CALL(GetString(str));
        utf8.clear();
        CALL(SaveUtf8(utf8, str));
        DONE;
    }
    PROC GetUuid(Uuid &value) const;

    PROC GetUuidInteger(Uuid &id, Integer &value) const;
    PROC GetUuidFloat(Uuid &id, Float &value) const;
    PROC GetUuidString(Uuid &id, span32c &value) const;
    PROC GetUuidUuid(Uuid &id, Uuid &value) const;

    PROC GetSingularString(Codepoint &value) const;
    PROC GetStringString(span32c &key, span32c &value) const;

    /** Whether it is a valid RON UUID */
    PROC Verify() const;

    std::string Debug() const;

    static PROC Parse(Tuple &into, const std::string &ront);

    static Tuple As(std::string ront) {
        Tuple ret{};
        Parse(ret, std::move(ront));
        return ret;
    }

    PROC ParseRONtAtom(span8c atom);
    /** A coercing atom parser; if the data can not be parsed as a valid
     *  RON atom, it is understood as an unquoted STRING. */
    PROC CoerceRONtAtom(span8c atom) {
        //        TRY(parsed, ParseRONtAtom(atom));
        //        if (parsed != OK) CALL(PushUtf8String(atom));
        //        DONE;
        FAIL(NOTIMPLTD);
    }
    PROC CoerceAtom(const std::string &atom) {
        return CoerceRONtAtom(as_span8c(atom));
    }

    Tuple &operator=(const std::string &ront) {
        (*this) = As(ront);
        return *this;
    }

    template <typename H>
    friend H AbslHashValue(H state, const Tuple &v) {
        auto d = v.Data();
        for (auto w : d) state = H::combine(std::move(state), w);
        return state;
    }

    bool operator==(const Box box) const {
        return IsSingleBox() && LastBox() == box;
    }

    PROC Has(Uuid id) const;
};

struct Spec {
    Uuid id, ref;
    bool operator==(const Spec &b) const { return id == b.id && ref == b.ref; }
    /** as raw bytes, RONc little-endian layout (find me a big endian CPU) */
    span8c Data8() const {
        return span8c{(uint8_t*)this, sizeof(Spec)};
    }
    void Clear() {
        id = NIL;
        ref = NIL;
    }
    void Next() {
        if (id.IsEvent()) {
            ref = id;
            ++id;
        }
    }
    bool IsChainedTo(const Uuid& bid) const {
        return ref == bid && id == bid.Inc();
    }
    bool IsChainedTo(const Spec& b) const {
        if (id.IsEvent()) {
            return IsChainedTo(b.id);
        } else {
            return *this == b;
        }
    }
};

/** A RONc op: immutable, consisting of a specifier (id, ref) and a value. */
class Op {
    Tuple values_;
    Spec meta_;

   public:
    // creates @0 :0;
    Op() = default;

    Op(Op &&b) noexcept : meta_{b.meta_}, values_{std::move(b.values_)} {}
    Op(const Op &b) = default;
    Op &operator=(const Op &b) noexcept {
        meta_ = b.meta_;
        values_ = b.Values();
        return *this;
    }

    Op(const Spec &meta) : meta_{meta}, values_{} {}
    Op(const Spec &meta, const Tuple &values) : meta_{meta}, values_{values} {}

    Op(Uuid op_id, Uuid causal_ref) : values_{}, meta_{op_id, causal_ref} {}
    Op(Uuid id, Uuid ref, Tuple val)
        : values_{std::move(val)}, meta_{id, ref} {}
    Op(Uuid id, Uuid ref, Tuple &&val)
        : values_{std::move(val)}, meta_{id, ref} {}

    Op &operator=(Op &&b) noexcept {
        meta_ = b.meta_;
        values_ = std::move(b.values_);
        return *this;
    }

    bool operator==(const Op &b) const {
        return meta_ == b.meta_ && values_ == b.Values();
    }

    template <typename... Head>
    Op(Uuid op_id, Uuid causal_ref, const Head... args)
        : values_{args...}, meta_{op_id, causal_ref} {}

    Uuid ID() const { return meta_.id; }
    Uuid &ID() { return meta_.id; }
    Uuid Ref() const { return meta_.ref; }
    Uuid &Ref() { return meta_.ref; }
    const Spec &Meta() const { return meta_; }
    Spec &Meta() { return meta_; }
    bool IsEmpty() const { return values_.IsEmpty() && meta_ == Spec{}; }
    bool IsChainedTo(const Spec& spec) const {
        return meta_.IsChainedTo(spec);
    }
    bool IsChainedTo(const Uuid& id) const {
        return meta_.IsChainedTo(id);
    }

    /** @deprecated */
    void SetID(Uuid id) { meta_.id = id; }
    void SetRef(Uuid ref) { meta_.ref = ref; }
    void SetSpec(Uuid id, Uuid ref) {
        SetID(id);
        SetRef(ref);
    }

    bool IsEvenChain() const { return ID() == Ref().Inc(); }

    /** singular op in an even chain */
    bool IsStraightChainOp() const {
        return values_.IsSingleAtom() && IsEvenChain();
    }

    bool IsEqual(const Op &b) const {
        return meta_ == b.meta_ && values_.IsEqual(b.values_);
    }

    void NextInEvenChain() { meta_.Next(); }

    void Clear() {
        meta_ = Spec{};
        values_.Clear();
    }

    Tuple &Values() { return values_; }
    const Tuple &Values() const { return values_; }

    static PROC Parse(Op &op, const std::string &ront);
};

using Ops = std::vector<Op>;
using Tuples = std::vector<Tuple>;

void Report(Status status);

}  // namespace ron

namespace std {

template <>
struct hash<ron::Tuple> {
    size_t operator()(const ron::Tuple &values) const {
        std::hash<uint64_t> h{};
        auto words = values.Data();
        size_t hash = 0;
        for (auto &w : words) hash ^= h(w);
        return hash;
    }
};

inline void swap(ron::Tuple &a, ron::Tuple &b) { a.Swap(b); }

};  // namespace std

#endif /* op_hpp */
