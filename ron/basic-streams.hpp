#ifndef RON_BASIC_STREAMS_HPP
#define RON_BASIC_STREAMS_HPP

#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstring>

#include "atom.hpp"
#include "bits.hpp"
#include "const.hpp"
#include "protobuf.hpp"
#include "./deps/ryu/ryu/ryu.h"
#include "stream.hpp"
#include "uuid.hpp"

namespace ron {

inline bool IsLastVarIntByte(uint8_t b) { return b < 0x80; }

/** A generic *paged* varint reader. This is an *overlay* datastructure, a
 * LEB128 varint stream. It has no idea about the underlying memory and the way
 * the new pages get obtained (read from a file, taken from a page chain,
 * unzipped, etc) Hence, any such details are left for descendant classes. */
class VarIntStream : public Stream {
   public:
   protected:
    PROC Drain32Unchecked(uint32_t &value);
    PROC Drain64Unchecked(uint64_t &value);

    PROC CheckDrainable() {
        auto c = Filled();
        if (likely(c.size_bytes() >= kMaxVarintBytes)) DONE;
        if (!c.empty() && IsLastVarIntByte(c.back())) DONE;
        return CheckDrainable(1);
    }

    PROC CheckDrainable(Index varint_count);

   public:
    VarIntStream() = default;
    template <typename... A>
    explicit VarIntStream(const A... a) : Stream{a...} {}
    VarIntStream(VarIntStream &&b) noexcept : Stream{std::move(b)} {}

    /** Convention: only 7-bit values (0..127) get written/read as uint8_t */
    PROC DrainByte(uint8_t &v) {
        CALL(CheckDrainable());
        auto c = Filled();
        NEED(IsLastVarIntByte(c.front()), BADRECORD.Ext(BASE_V));
        v = c.front();
        Drained(1);
        DONE;
    }

    PROC FeedIndex(Index v) {
        if (unlikely(IdleSize() < kMaxVarint32Bytes)) CALL(ReserveSpace());
        auto l = UnsafeVarint(v, Idle().data());
        Fed(l);
        DONE;
    }

    /** Reads `uint32` in LEB128 coding */
    PROC DrainIndex(Index &v) {
        CALL(CheckDrainable());
        auto c = Filled();
        if (!IsLastVarIntByte(c.front())) return Drain32Unchecked(v);
        v = c.front();
        Drained(1);
        DONE;
    }

    PROC FeedCodepoint(Codepoint cp) {
        if (unlikely(IdleSize() < kMaxVarint32Bytes)) CALL(ReserveSpace());
        auto l = UnsafeVarint(cp, Idle().data());
        Fed(l);
        DONE;
    }

    PROC DrainCodepoint(Codepoint &cp) {
        CALL(CheckDrainable());
        auto c = Filled();
        if (!IsLastVarIntByte(c.front()))
            return Drain32Unchecked((uint32_t &)cp);
        cp = c.front();
        Drained(1);
        DONE;
    }

    PROC FeedWord(Word v) {
        if (unlikely(IdleSize() < kMaxVarintBytes)) CALL(ReserveSpace());
        auto l = UnsafeVarint(v, Idle().data());
        Fed(l);
        DONE;
    }

    /** Reads `uint64` in LEB128 coding */
    PROC DrainWord(Word &v) {
        CALL(CheckDrainable());
        auto c = Filled();
        if (!IsLastVarIntByte(c.front())) return Drain64Unchecked(v);
        v = c.front();
        Drained(1);
        DONE;
    }

    PROC FeedInteger(Integer v) { return FeedWord(ZigZagEncode64(v)); }

    PROC DrainInteger(Integer &v) {
        uint64_t tmp;
        CALL(DrainWord(tmp));
        v = ZigZagDecode64(tmp);
        DONE;
    }

    PROC FeedWordFlipped(Word f) { return FeedWord(flip64(f)); }

    PROC DrainFlippedWord(Word &v) {
        uint64_t tmp;
        CALL(DrainWord(tmp));
        v = flip64(tmp);
        DONE;
    }

    PROC FeedFloat(Float f) { return FeedWordFlipped(as_word(f)); }
    PROC DrainFloat(Float &v) { return DrainFlippedWord(as_word(v)); }

    PROC FeedUuid(Uuid id) {
        CALL(FeedWord(flip64(id.second)));
        if (id.Version() != YOLO) return FeedWord(flip64(id.first));
        uint32_t hndx = id.first >> INDEX_PAYLOAD_BIT_SIZE,
                 yndx = id.first & INDEX_PAYLOAD_BITS;
        CALL(FeedIndex(hndx));
        CALL(FeedIndex(yndx));
        DONE;
    }

    PROC DrainUuid(Uuid &id) {
        CALL(DrainFlippedWord(id.second));
        NEED((id.second >> 62U) == 0, BADRECORD.Ext(BASE_U));
        if (id.Version() != YOLO) return DrainFlippedWord(id.first);
        uint32_t hndx, yndx;
        CALL(DrainIndex(hndx));
        CALL(DrainIndex(yndx));
        id.first = (uint64_t(hndx) << INDEX_PAYLOAD_BIT_SIZE) | yndx;
        DONE;
    }

    PROC FeedBox(Tuple::Box box);

    PROC DrainBox(BoxDescriptor &desc, Tuple &into);

    /** whether the given pos is valid (a beginning of a varint or the end) */
    static bool IsValidPosition(span8c data, span8c::index_type pos) {
        if (pos >= data.size()) return pos == data.size();
        if (IsLastVarIntByte(data[pos])) return true;
        return pos == 0 || data[pos - 1] < 0x80;
    }
};

/** A text stream. Can parse primitives: ints, floats, RON Base64 ints.
 *  Supports UTF8. Depends on an external lexer. */
class TextStream : public Stream {
    static bool IsAmbiguousFloatAsInt(span8c data);

    PROC FeedRONEscapedCodepointLong(Codepoint cp);

   public:
    TextStream() = default;
    TextStream(TextStream &&b) noexcept : Stream{std::move(b)} {}
    template <typename... A>
    explicit TextStream(A... args) : Stream{args...} {}

    PROC FeedChar(char c) { return FeedByte(c); }

    PROC DrainInteger(Integer &value, span8c::iterator end) {
        uint8_t *e;
        errno = 0;
        value = std::strtoll((char *)Filled().data(), (char **)&e, 10);
        if (errno == ERANGE) FAIL(BADSYNTAXI);
        Drained(end);
        return e == end ? OK : BADSYNTAXI;
    }

    PROC DrainFloat(Float &value, span8c::iterator end) {
        uint8_t *e;
        value = strtod((char *)At(), (char **)&e);
        if (std::isnan(value) || std::isinf(value)) FAIL(BADSYNTAX.Ext(BASE_F));
        Drained(end);
        return e == end ? OK : BADSYNTAX.Ext(BASE_F);
    }

    PROC DrainIndex(Index &value, span8c::iterator end) {
        assert(end <= Filled().end());
        auto i = At();
        auto s = end - i;
        NEED(s > 0 && s <= MAX_BASE64_INDEX_SIZE, BADSYNTAXU);
        value = 0;
        while (i < end) {
            value <<= 6U;
            value |= ABC64[*i];
            ++i;
        }
        Drained(end);
        DONE;
    }

    PROC FeedWord(Word value) {
        CALL(ReserveSpace(MAX_BASE64_WORD_SIZE));
        auto f = Idle();
        Uuid::SaveWord(f, value);
        Fed(f.begin());
        DONE;
    }

    /** all 10 chars */
    PROC FeedPaddedWord(Word value) {
        uint8_t buf[11];
        memset(buf, '0', 10);
        auto *e = buf;
        value <<= 4U;  // flags, varieties
        do {
            *e++ = (uint8_t)BASE_PUNCT[0x3fU & (value >> 58U)];
            value <<= 6U;
        } while (value);
        CALL(ReserveSpace(kMaxVarintBytes));
        return FeedSpan(span8c{buf, 10});
    }

    PROC DrainWord(Word &value, span8c::iterator end) {
        assert(end <= Filled().end());
        auto i = At();
        auto s = end - i;
        NEED(s > 0 && s <= MAX_BASE64_WORD_SIZE, BADSYNTAXU);
        value = 0;
        uint32_t shift{60};
        while (i < end) {
            value <<= 6U;
            value |= ABC64[*i];
            shift -= 6U;
            ++i;
        }
        value <<= shift;
        Drained(end);
        DONE;
    }

    PROC FeedIndex(Index value) {
        CALL(ReserveSpace(MAX_BASE64_INDEX_SIZE))
        auto f = Idle();
        auto d = To();
        if (value > (1 << 24) - 1) *d++ = BASE_PUNCT[(value >> 24) & 63];
        if (value > (1 << 18) - 1) *d++ = BASE_PUNCT[(value >> 18) & 63];
        if (value > (1 << 12) - 1) *d++ = BASE_PUNCT[(value >> 12) & 63];
        if (value > (1 << 6) - 1) *d++ = BASE_PUNCT[(value >> 6) & 63];
        *d++ = BASE_PUNCT[value & 63];
        Fed(d);
        DONE;
    }

    PROC FeedInteger(Integer value) {
        CALL(ReserveSpace(32));
        char tmp[32];
        span8 ::index_type len = sprintf(tmp, "%" PRId64, value);
        return FeedDataUnsafe(span8c{(uint8_t *)tmp, len});
    }

    PROC FeedFloat(Float value) {
        static constexpr unsigned char E0[3]{"E0"};
        CALL(ReserveSpace(32));
        uint8_t tmp[32];
        // floats are a pain; esp converting binary<->text
        // our only interest here is to preserve the hashes,
        // so binary->text->binary must not change the value;
        // text->binary->text we don't care about (see test/text.cc)
        // 17 is DBL_DECIMAL_DIG, enough to express 64 bit ISO floats
        span8 ::index_type len = d2s_buffered_n(double(value), (char *)tmp);
        if (span8(tmp + len - 2, 2) == span8{(uint8_t *)E0, 2}) {
            len -= 2;
        }
        if (IsAmbiguousFloatAsInt(span8{tmp, len})) {
            tmp[len++] = '.';
            tmp[len++] = '0';
        }
        return FeedDataUnsafe(span8c{tmp, len});
    }

    PROC FeedUuid(Uuid id) {
        CALL(ReserveSpace(Uuid::MAX_BASE64_SIZE));
        auto idle = Idle();
        CALL(Uuid::Save(idle, id));
        Fed(idle.begin());
        DONE;
    }

    PROC DrainDoubleWord(Double &value, span8c::iterator end) {
        auto f = At();
        if (end - f > 10) {
            CALL(DrainWord(value.first, f + 10));
            return DrainWord(value.second, end);
        } else {
            value.second = 0;
            return DrainWord(value.first, end);
        }
    }

    PROC DrainAscii(Codepoint &cp) {
        assert(*At() < 128);
        cp = *At();
        Drained(1);
        DONE;
    }

    PROC FeedRONEscapedCodepoint(Codepoint cp) {
        if (cp < 0x80 && cp >= 0x20 && cp != '\'' && cp != '\\') {
            CALL(FeedByte(cp));  // TODO
        } else {
            CALL(FeedRONEscapedCodepointLong(cp));
        }
        DONE;
    }
    PROC FeedRONEscapedCodepoints(span32c str) {
        for (auto cp : str) CALL(FeedRONEscapedCodepoint(cp));
        DONE;
    }

    PROC FeedJSONEscapedCodepointLong(Codepoint cp);
    PROC FeedJSONEscapedCodepoint(Codepoint cp) {
        if (cp < 0x80 && cp >= 0x20 && cp != ASCII::DBLQUOTE &&
            cp != ASCII::BSLASH) {
            CALL(FeedByte(cp));
        } else {
            CALL(FeedJSONEscapedCodepointLong(cp));
        }
        DONE;
    }
    template <unsigned L>
    PROC DrainUtf8(Codepoint &cp, span8c::iterator end) {
        if (L == 1U) return DrainAscii(cp);
        auto b = At();
        cp = *b & (127U >> L);
        ++b;
        for (int i = 1; i < L; ++i) {
            cp = (cp << 6U) | (*b++ & 63U);
        }
        if (L > 2 && cp >= 0xD800 && cp < 0xE000) FAIL(BADUNICODE.Ext(BASE_P));
        if (L > 3 && cp > MAX_UNICODE_CODEPOINT) FAIL(BADUNICODE.Ext(BASE_R));
        assert(b == end);
        Drained(b);
        DONE;
    }

    PROC DrainUnicodeEscape(Codepoint &cp, span8c::iterator end);

    PROC DrainAsciiEscape(Codepoint &cp, span8c::iterator end);

    PROC DrainCoercedAtom(Atom &atom, span8c::iterator end);
};

}  // namespace ron

#endif
