//
//  uuid.hpp
//  pulsar
//
//  Created by gritzko gritzko on 01/09/2019.
//  Copyright © 2019 gritzko. All rights reserved.
//

#ifndef uuid_hpp
#define uuid_hpp

#include <cstring>
#include <string>
#include <vector>

#include "bits.hpp"
#include "const.hpp"
#include "protobuf.hpp"
#include "utf8.hpp"

namespace ron {

struct Uuid : public Double {
    enum VARIANT : uint8_t {
        RON = 0,
        DELTA = 1,
        RFC4122 = 2,
        MSRESERVE = 3,
    };

    inline Uuid() = default;
    inline Uuid(const Uuid& b) = default;
    explicit inline Uuid(const Double& bits) : Double{bits} {}
    inline Uuid(Word first, Word second) : Double{first, second} {}
    explicit Uuid(span8c data) : Uuid{} {
        if (OK != ParseUuid(*this, data))
            *this = Uuid{WORD_PAYLOAD_BITS, WORD_PAYLOAD_BITS};
    }
    explicit Uuid(const std::string& data) : Uuid{as_span8c(data)} {}

    UUID Version() const { return UUID((second >> 60U) & 3U); }
    bool IsEvent() const { return second >> 61U; }
    VARIANT Variant() const { return VARIANT(second >> 62U); }
    Word Origin() const { return second & WORD_PAYLOAD_BITS; }
    bool IsValid() { return (second >> 62U) == ID; }
    bool IsShortName() const { return second == 0; }
    bool IsVeryShortName() const {
        constexpr Word LOW5 = (1UL << 30) - 1;
        return IsShortName() && (second & LOW5) == 0;
    }
    span64 AsWordSpan() const { return span64{(Word*)this, 2}; }
    Index& UpperIndex() { return Halves::Of(first).ms_half; }
    Index& LowerIndex() { return Halves::Of(first).ls_half; }
    Index HomeIndex() const {
        assert(Version() == YOLO);
        return Halves::Of(first).ms_half;
    }
    Index YarnIndex() const {
        assert(Version() == YOLO);
        return Halves::Of(first).ls_half;
    }
    Index& HomeIndex() {
        assert(Version() == YOLO);
        return Halves::Of(first).ms_half;
    }
    Index& YarnIndex() {
        assert(Version() == YOLO);
        return Halves::Of(first).ls_half;
    }

    Word Value() const {
        assert(Version() != YOLO);
        return first & WORD_PAYLOAD_BITS;
    }

    void ToString(std::string& ret) const {
        ret.resize(MAX_BASE64_SIZE);
        auto s = as_span8(ret);
        Save(s, *this);
        ret.resize(MAX_BASE64_SIZE - s.size());
    }

    std::string String() const {
        std::string ret{};
        ToString(ret);
        return ret;
    }

    /**  */
    static PROC SaveIndex(span8& into, Index value) {
        assert(into.size() >= MAX_BASE64_INDEX_SIZE);
        auto d = into.data();
        if (value > (1 << 24) - 1) *d++ = BASE_PUNCT[(value >> 24) & 63];
        if (value > (1 << 18) - 1) *d++ = BASE_PUNCT[(value >> 18) & 63];
        if (value > (1 << 12) - 1) *d++ = BASE_PUNCT[(value >> 12) & 63];
        if (value > (1 << 6) - 1) *d++ = BASE_PUNCT[(value >> 6) & 63];
        *d++ = BASE_PUNCT[value & 63];
        into = span8{d, into.end()};
        DONE;
    }

    static PROC ParseIndex(Index& value, span8c data) {
        NEED(!data.empty() && data.size() <= MAX_BASE64_INDEX_SIZE, BADSYNTAXU);
        value = 0;
        for (auto i : data) {
            value <<= 6U;
            value |= ABC64[i];
        }
        DONE;
    }

    /** parses a RON Base64 coded word */
    static PROC ParseWord(Word& value, span8c data);

    static PROC ParseDouble(Double& value, span8c data);

    static PROC ParseUuid(Uuid& id, span8c data);

    static PROC SaveWord(span8& buf, Word value) {
        auto b = buf.begin();
        value <<= 4U;  // flags, varieties
        do {
            *b = BASE_PUNCT[0x3fU & (value >> 58U)];
            value <<= 6U;
            ++b;
        } while (value);
        buf = buf.subspan(b - buf.begin());
        DONE;
    }

    static PROC SavePaddedWord(span8& buf, Word value) {
        auto b = buf.begin();
        value <<= 4U;  // flags, varieties
        for (int i = 0; i < MAX_BASE64_WORD_SIZE; ++i) {
            *b = BASE_PUNCT[0x3fU & (value >> 58U)];
            value <<= 6U;
            ++b;
        }
        buf = buf.subspan(MAX_BASE64_WORD_SIZE);
        DONE;
    }

    static PROC Save(span8& into, Uuid value);

    static PROC Save(std::string& str, Uuid value) {
        str.clear();
        str.resize(MAX_BASE64_SIZE);
        auto s = as_span8(str);
        CALL(Save(s, value));
        str.resize(MAX_BASE64_SIZE - s.size());
        DONE;
    }

    inline Uuid operator++();
    inline Uuid Inc() const {
        Uuid i{*this};
        ++i;
        return i;
    }

    static constexpr uint64_t YOLO_INC{1UL | (1UL << INDEX_PAYLOAD_BIT_SIZE)};
    static constexpr int MAX_BASE64_SIZE = 21;

    static Uuid Name(Double long_name) { return Uuid{long_name}; }
    static Uuid Name(Word short_name) { return Uuid{short_name, NAME_FLAGS}; }
    static Uuid Name(Status error) { return Uuid{error._s, NAME_FLAGS}; }
    static Uuid Name(Word first, Word second) {
        return Uuid{first, second | NAME_FLAGS};
    }

    static Uuid Tag(Word short_name, Word name_scope) {
        return Uuid{short_name, name_scope | TAG_FLAGS};
    }
    static Uuid Scoped(Word short_name, Index row, Index col) {
        return Tag(short_name, (row << 30) | col);
    }

    static Uuid Lamport(Word logical_clock, Word origin_replica) {
        return Uuid{logical_clock, origin_replica | LAMPORT_FLAGS};
    }

    static Uuid Yolo(Index home_index, Index yarn_index, Word origin_replica) {
        return Uuid{Halves(yarn_index, home_index).word,
                    origin_replica | YOLO_FLAGS};
    }

    bool IsAmbiguous() const {
        constexpr uint64_t HAS_LETTERS = 878416384462359600UL;
        constexpr uint64_t HAS_LETTERS_BUT_E =
            (272696336UL << 30) | 272696336UL;
        switch (Version()) {
            case TAG:
            case YOLO:
                return false;
            case NAME:  // 2e1
                if (first & HAS_LETTERS_BUT_E) return false;
                if (second & HAS_LETTERS_BUT_E) return false;
                return IsAmbiguousAsNumber(first) &&
                       IsAmbiguousAsNumber(second);
            case LAMPORT:  // 2e+1
                if (second & HAS_LETTERS) return false;
                if (first & HAS_LETTERS_BUT_E) return false;
                return IsAmbiguousAsNumber(first) &&
                       IsAmbiguousAsNumber(second);
        }
    }

    static bool IsAmbiguousAsNumber(Word w);

    static PROC ApplyDelta(Uuid& id, const Uuid& delta, const Uuid& base) {
        NEED(delta.Version() == base.Version(), BADUIDELT.Ext(BASE_U));
        switch (base.Version()) {
            case LAMPORT:
                id =
                    Uuid(base.first + ZigZagDecode64(delta.first), base.second);
                DONE;
            case YOLO:
                id = Yolo(base.HomeIndex() + ZigZagDecode64(delta.HomeIndex()),
                          base.YarnIndex() + ZigZagDecode64(delta.YarnIndex()),
                          base.second);
                DONE;
            default:
                if (delta.first) FAIL(BADUIDELT.Ext(BASE_U));
                id = base;
                DONE;
        }
    }

    static PROC Delta(Uuid& delta, const Uuid& id, const Uuid& base) {
        static constexpr Word DELTA_FLAGS = Word(DELTA) << 62;
        NEED(id.second == base.second, BADUIDELT.Ext(BASE_o));
        switch (id.Version()) {
            case NAME:
                if (id != base) FAIL(BADUIDELT.Ext(BASE_n));
                delta = Uuid{0, NAME_FLAGS | DELTA_FLAGS};
                DONE;
            case TAG:
                if (id != base) FAIL(BADUIDELT.Ext(BASE_t));
                delta = Uuid{0, TAG_FLAGS | DELTA_FLAGS};
                DONE;
            case YOLO:
                delta =
                    Uuid(Halves{(uint32_t)ZigZagEncode64(
                                    int64_t(id.YarnIndex()) - base.YarnIndex()),
                                (uint32_t)ZigZagEncode64(
                                    int64_t(id.HomeIndex()) - base.HomeIndex())}
                             .word,
                         YOLO_FLAGS | DELTA_FLAGS);
                DONE;
            case LAMPORT:
                delta = Uuid(ZigZagEncode64(int64_t(id.first) - base.first),
                             LAMPORT_FLAGS | DELTA_FLAGS);
                DONE;
            default:
                FAIL(BADSTATE);
        }
    }

    PROC Verify() const {
        NEED(Variant() == RON, BADUUID.Ext(BASE_V));
        if (Version() == YOLO) {
            NEED(HomeIndex() <= MAX_INDEX, BADUUID.Ext(BASE_h));
            NEED(YarnIndex() <= MAX_INDEX, BADUUID.Ext(BASE_y));
        } else {
            // TODO the m.s. 4 bits of the value are reserved
            NEED(first <= WORD_PAYLOAD_BITS, BADUUID.Ext(BASE_v));
        }
        DONE;
    }

    Uuid YoloDec(Index step) const {
        assert(Version() == YOLO);
        Uuid b{*this};
        b.first -= step;
        b.first -= Word{step} << INDEX_BIT_SIZE;
        return b;
    }

    /** @return time UUID, like 20210216~202124 */
    static Uuid Time();
};

using Uuids = std::vector<Uuid>;

const Uuid YOLO_ZERO = Uuid::Yolo(0, 0, 0);
const Uuid LAMPORT_ZERO = Uuid::Lamport(0, 0);
const Uuid TAG_ZERO = Uuid::Tag(0, 0);
const Uuid FATAL = Uuid::Name(WORD_PAYLOAD_BITS, WORD_PAYLOAD_BITS);
const Uuid NIL = Uuid::Name(0UL, 0UL);

constexpr Word YARN_WORD{1109533813702131712UL};
constexpr Word CT_WORD{718324140565594112UL};
constexpr Word LWW_WORD{881557636825219072UL};
const Uuid YARN_RDT = Uuid::Name(YARN_WORD, 0UL);
const Uuid LWW_RDT = Uuid::Name(LWW_WORD, 0);
const Uuid CT_RDT = Uuid::Name(CT_WORD, 0);
const Uuid BLOB_RDT = Uuid::Name(698284853954609152UL);
constexpr Word WORD_TXT{1025941105738252288UL};

// ABC has char-to-code conversions for all of Text RON punctuation
extern const int8_t ABC64[256];

inline std::string Word2String(Word value) {
    std::string ret{};
    ret.resize(MAX_BASE64_WORD_SIZE);
    auto s = as_span8(ret);
    Uuid::SaveWord(s, value);
    ret.resize(MAX_BASE64_WORD_SIZE - s.size());
    return ret;
}

inline std::string Status::String() const { return Word2String(_s); }

inline Uuid Uuid::operator++() {
    static constexpr Word YOLO_BAD_BITS = (3UL << 30U) | (3UL << 62U);
    static constexpr Word YOLO_MAX = (0x3FFFFFFFUL << 32U) | 0x3FFFFFFFUL;
    switch (Version()) {
        case LAMPORT:
            first = (first & WORD_FLAG_BITS) | ((first + 1) & MAX_WORD);
            break;
        case YOLO:
            first = (first + Halves::DOUBLE_INC) & YOLO_MAX;  // wrap-around
            break;
        default:
            break;
    }
    return *this;
}

/** The origin-keyed micro-hash-table for UUID caching; */
struct MicroCache {
    static const int CACHE_SIZE =
        16;                  // have 4 free bits in the origin's m.s.byte
    Uuid hash_[CACHE_SIZE];  // 1/2 KB
    Index CellFor(Uuid id) { return WordMix(id.second) & (CACHE_SIZE - 1); }
    void Store(Uuid id) { hash_[CellFor(id)] = id; }
    /** @return OK, BADUUIDLT */
    PROC ResolveDelta(Uuid& id, Uuid delta) {
        auto cell = (delta.second >> 56) & (CACHE_SIZE - 1);
        return Uuid::ApplyDelta(id, delta, hash_[cell]);
    }
    /** @return OK, BADUUIDLT */
    PROC MakeDelta(Uuid& delta, Uuid id) {
        auto c = CellFor(id);
        CALL(Uuid::Delta(delta, id, hash_[c]));
        delta.second |= Word(c) << 56;
        DONE;
    }
    void Clear() { memset(hash_, 0, sizeof(hash_)); }
};

}  // namespace ron

namespace std {

template <>
struct hash<ron::Uuid> {
    size_t operator()(const ron::Uuid& id) const {
        return ron::WordMix(id.first) ^ ron::WordMix(id.second);
    }
};

}  // namespace std
#endif /* uuid_hpp */
