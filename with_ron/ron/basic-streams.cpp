//
// Created by gritzko on 5/1/20.
//
#include "basic-streams.hpp"

namespace ron {

PROC VarIntStream::Drain64Unchecked(uint64_t &value) {
    auto inc = ReadVarint64FromArray(Filled().data(), value);
    if (unlikely(inc == 0)) FAIL(BADRECORD.Ext(3));
    Drained(inc);
    DONE;
}

PROC VarIntStream::Drain32Unchecked(uint32_t &value) {
    assert(Filled().front() >= 128);
    auto be = Filled().begin();
    auto ptr = be;
    uint32_t b;
    uint32_t result = *ptr - 0x80;
    ++ptr;  // We just processed the first byte.  Move on to the second.
    b = *(ptr++);
    result += b << 7U;
    if (!(b & 0x80U)) goto done;
    result -= 0x80U << 7U;
    b = *(ptr++);
    result += b << 14U;
    if (!(b & 0x80U)) goto done;
    result -= 0x80U << 14U;
    b = *(ptr++);
    result += b << 21U;
    if (!(b & 0x80U)) goto done;
    result -= 0x80U << 21U;
    b = *(ptr++);
    result += b << 28U;
    if (!(b & 0x80U)) goto done;
    // "result -= 0x80 << 28" is irrevelant.
    // If the input is larger than 32 bits, we consider the data corrupt!
    FAIL(BADRECORD.Ext(BASE_v));

done:
    value = result;
    Drained(ptr - be);
    DONE;
}

PROC VarIntStream::FeedBox(Tuple::Box box) {
    CALL(FeedWord(box.Descriptor().Coded()))
    Index ndx{0};
    switch (box.Type()) {
        case INT:
            do {
                CALL(FeedInteger(box.IntegerAt(ndx)));
            } while (++ndx < box.Size());
            DONE;
        case STRING:
            while (ndx < box.Size()) {
                CALL(FeedCodepoint(box.CodepointAt(ndx)));
                ++ndx;
            }
            DONE;
        case FLOAT:
            do {
                CALL(FeedFloat(box.FloatAt(ndx)));
            } while (++ndx < box.Size());
            DONE;
        case ID:
            do {
                CALL(FeedUuid(box.UuidAt(ndx)));
            } while (++ndx < box.Size());
            DONE;
        default:
            abort();
    }
    DONE;
}

PROC VarIntStream::DrainBox(BoxDescriptor &desc, Tuple &tuple) {
    // RONv is an external format, so the input has to be
    // checked, sanitized and normalized!
    CALL(VarIntStream::DrainWord(desc.coded_));
    auto size = desc.Size();
    span64 rest;
    span32 cps;
    CALL(tuple.PushNewBox(rest, desc));
    switch (desc.Type()) {
        case INT:
            NEED(size > 0, BADRECORD.Ext(BASE_T));
            for (int j = 0; j < size; ++j)
                CALL(VarIntStream::DrainInteger((Integer &)rest[j]));
            break;
        case FLOAT:
            NEED(size > 0, BADRECORD.Ext(BASE_T));
            for (int j = 0; j < size; ++j)
                CALL(VarIntStream::DrainFloat((Float &)rest[j]));
            break;
        case ID:
            for (int j = 0; j < (size << 1U); j += 2)
                CALL(VarIntStream::DrainUuid((Uuid &)(rest.data()[j])));
            break;
        case STRING:
            // empty STRING is OK
            if (!size) break;
            cps = as_span32(rest);
            cps.back() = 0;  // padding
            for (auto cp : cps) CALL(VarIntStream::DrainCodepoint(cp));
            break;
        default:
            abort();
    }
    DONE;
}

bool TextStream::IsAmbiguousFloatAsInt(span8c data) {
    for (auto i = data.begin(); i != data.end(); ++i) {
        switch (*i) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '-':
            case '+':
                continue;
            default:
                return false;
        }
    }
    return true;
}

PROC TextStream::FeedRONEscapedCodepointLong(Codepoint cp) {
    constexpr int MAX_CP_LEN = 8;
    uint8_t into[MAX_CP_LEN];
    uint8_t *i = into;
    CALL(ReserveSpace(MAX_CP_LEN));
    if (cp < 0x80) {
        switch (uchar8(cp)) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 14:
            case 15:
            case 16:
            case 17:
            case 18:
            case 19:
            case 20:
            case 21:
            case 22:
            case 23:
            case 24:
            case 25:
            case 26:
            case 28:
            case 29:
            case 30:
            case 31:
                *i++ = ASCII::BSLASH;
                *i++ = 'u';
                *i++ = '0';
                *i++ = '0';
                *i++ = BASE_PUNCT[cp >> 4U];
                *i++ = BASE_PUNCT[cp & 0xfU];
                break;
            case ASCII::NUL:
                *i++ = ASCII::BSLASH;
                *i++ = '0';
                break;
            case ASCII::BS:
                *i++ = ASCII::BSLASH;
                *i++ = 'b';
                break;
            case ASCII::HT:
                *i++ = ASCII::BSLASH;
                *i++ = 't';
                break;
            case ASCII::LF:
                *i++ = ASCII::BSLASH;
                *i++ = 'n';
                break;
            case ASCII::ESC:
                *i++ = ASCII::BSLASH;
                *i++ = 'e';
                break;
            case ASCII::CR:
                *i++ = ASCII::BSLASH;
                *i++ = 'r';
                break;
            case ASCII::FF:
                *i++ = ASCII::BSLASH;
                *i++ = 'f';
                break;
            case ASCII::BEL:
                *i++ = ASCII::BSLASH;
                *i++ = 'a';
                break;
            case ASCII::VT:
                *i++ = ASCII::BSLASH;
                *i++ = 'v';
                break;
            case ASCII::QUOTE:
                *i++ = ASCII::BSLASH;
                *i++ = ASCII::QUOTE;
                break;
            case ASCII::BSLASH:
                *i++ = ASCII::BSLASH;
                *i++ = ASCII::BSLASH;
                break;
            default:
                *i++ = cp;
                break;
        }
    } else if (cp < 0x800) {
        *i++ = (uchar8(cp >> 6U) | UTF8LEAD2);
        *i++ = (uchar8(cp & 63U) | UTF8CONT);
    } else if (cp < 0x10000) {
        *i++ = (uchar8(cp >> 12U) | UTF8LEAD3);
        *i++ = (uchar8((cp >> 6U) & 63U) | UTF8CONT);
        *i++ = (uchar8(cp & 63U) | UTF8CONT);
    } else if (cp <= MAX_UNICODE_CODEPOINT) {
        *i++ = (uchar8(cp >> 18U) | UTF8LEAD4);
        *i++ = (uchar8((cp >> 12U) & 63U) | UTF8CONT);
        *i++ = (uchar8((cp >> 6U) & 63U) | UTF8CONT);
        *i++ = (uchar8(cp & 63U) | UTF8CONT);
    } else {
        assert(false);
    }
    return FeedDataUnsafe(span8c{into, i});
}

PROC TextStream::FeedJSONEscapedCodepointLong(Codepoint cp) {
    constexpr int MAX_CP_LEN = 8;
    uint8_t into[MAX_CP_LEN];
    uint8_t *i = into;
    CALL(ReserveSpace(MAX_CP_LEN));
    if (cp < 0x80) {
        switch (uchar8(cp)) {
            case ASCII::BS:
                *i++ = ASCII::BSLASH;
                *i++ = 'b';
                break;
            case ASCII::HT:
                *i++ = ASCII::BSLASH;
                *i++ = 't';
                break;
            case ASCII::LF:
                *i++ = ASCII::BSLASH;
                *i++ = 'n';
                break;
            case ASCII::CR:
                *i++ = ASCII::BSLASH;
                *i++ = 'r';
                break;
            case ASCII::FF:
                *i++ = ASCII::BSLASH;
                *i++ = 'f';
                break;
            case ASCII::BSLASH:
                *i++ = ASCII::BSLASH;
                *i++ = ASCII::BSLASH;
                break;
            case ASCII::DBLQUOTE:
                *i++ = ASCII::BSLASH;
                *i++ = ASCII::DBLQUOTE;
                break;
            case ASCII::SLASH:
                *i++ = ASCII::BSLASH;
                *i++ = ASCII::SLASH;
                break;
            default:
                *i++ = ASCII::BSLASH;
                *i++ = 'u';
                *i++ = '0';
                *i++ = '0';
                *i++ = BASE_PUNCT[cp >> 4U];
                *i++ = BASE_PUNCT[cp & 0xfU];
                break;
        }
    } else if (cp < 0x800) {
        *i++ = (uchar8(cp >> 6U) | UTF8LEAD2);
        *i++ = (uchar8(cp & 63U) | UTF8CONT);
    } else if (cp < 0x10000) {
        *i++ = (uchar8(cp >> 12U) | UTF8LEAD3);
        *i++ = (uchar8((cp >> 6U) & 63U) | UTF8CONT);
        *i++ = (uchar8(cp & 63U) | UTF8CONT);
    } else if (cp <= MAX_UNICODE_CODEPOINT) {
        *i++ = (uchar8(cp >> 18U) | UTF8LEAD4);
        *i++ = (uchar8((cp >> 12U) & 63U) | UTF8CONT);
        *i++ = (uchar8((cp >> 6U) & 63U) | UTF8CONT);
        *i++ = (uchar8(cp & 63U) | UTF8CONT);
    } else {
        assert(false);
    }
    return FeedDataUnsafe(span8c{into, i});
}

PROC TextStream::DrainUnicodeEscape(Codepoint &cp, span8c::iterator end) {
    auto i = Filled().begin();
    assert(end-i==6); //?
    i += 2;
    cp = 0;
    while (i < end) {
        cp <<= 4U;
        cp |= ABC16[*i];
        ++i;
    }
    Drained(i);
    DONE;
}

PROC TextStream::DrainAsciiEscape(Codepoint &cp, span8c::iterator end) {
    auto i = Filled().begin();
    ++i;
    auto fc = *i;
    ++i;
    assert(i == end);
    Drained(end);
    switch (fc) {
        case '0':
            cp = ASCII::NUL;
            DONE;
        case 'n':
            cp = ASCII::LF;
            DONE;
        case 'r':
            cp = ASCII::CR;
            DONE;
        case 't':
            cp = ASCII::HT;
            DONE;
        case 'b':
            cp = ASCII::BS;
            DONE;
        case 'f':
            cp = ASCII::FF;
            DONE;
        case 'a':
            cp = ASCII::BEL;
            DONE;
        case 'e':
            cp = ASCII::ESC;
            DONE;
        case 'v':
            cp = ASCII::VT;
            DONE;
        case '\'':
        case '\\':
        case '/':
        case '"':
            cp = fc;
            DONE;
        default:
            FAIL(BADSYNTAX.Ext(BASE_e));  // cp==fc ?
    }
}

bool is_varint_last_byte(uint8_t b) { return b < 128; }

PROC VarIntStream::CheckDrainable(Index varint_count) {
    Index found = 0;
    auto c = Filled();
    for (auto i = c.begin(); i < c.end() && found < varint_count; ++i)
        if (IsLastVarIntByte(*i)) ++found;
    NEED (found >= varint_count, NODATA);
    DONE;
}

}  // namespace ron