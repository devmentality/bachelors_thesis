#include "utf8.hpp"

#include "const.hpp"

namespace ron {

PROC SaveUtf8Esc(std::string& into, span32c data) {
    for (auto& i : data) {
        SaveCodepointEsc(into, i);
    }
    DONE;
}

PROC SaveUtf8(std::string& into, span32c data) {
    for (auto i = data.begin(); i != data.end(); ++i) {
        SaveUtf8Codepoint(into, *i);
    }
    DONE;
}

PROC SaveUtf16(string16& into, span32c data) {
    for (auto cp : data) {
        if (cp > UINT16_MAX) FAIL(NOTIMPLTD);
        into.push_back(cp);
    }
    DONE;
}

void SaveUtf8CodepointLong(std::string& into, Codepoint cp) {
    assert(cp >= 0x80);
    if (cp < 0x800) {
        into.push_back(uchar8(cp >> 6) | UTF8LEAD2);
        into.push_back(uchar8(cp & 63) | UTF8CONT);
    } else if (cp < 0x10000) {
        into.push_back(uchar8(cp >> 12) | UTF8LEAD3);
        into.push_back(uchar8((cp >> 6) & 63) | UTF8CONT);
        into.push_back(uchar8(cp & 63) | UTF8CONT);
    } else if (cp <= MAX_UNICODE_CODEPOINT) {
        into.push_back(uchar8(cp >> 18) | UTF8LEAD4);
        into.push_back(uchar8((cp >> 12) & 63) | UTF8CONT);
        into.push_back(uchar8((cp >> 6) & 63) | UTF8CONT);
        into.push_back(uchar8(cp & 63) | UTF8CONT);
    } else {
        assert(false);
    }
}

void SaveUtf8CodepointLong(span8& into, Codepoint cp) {
    assert(cp >= 0x80);
    if (cp < 0x800) {
        into[0] = uchar8(cp >> 6) | UTF8LEAD2;
        into[1] = uchar8(cp & 63) | UTF8CONT;
        into = into.subspan(1);
    } else if (cp < 0x10000) {
        into[0] = uchar8(cp >> 12) | UTF8LEAD3;
        into[1] = uchar8((cp >> 6) & 63) | UTF8CONT;
        into[2] = uchar8(cp & 63) | UTF8CONT;
        into = into.subspan(3);
    } else if (cp <= MAX_UNICODE_CODEPOINT) {
        into[0] = uchar8(cp >> 18) | UTF8LEAD4;
        into[1] = uchar8((cp >> 12) & 63) | UTF8CONT;
        into[2] = uchar8((cp >> 6) & 63) | UTF8CONT;
        into[3] = uchar8(cp & 63) | UTF8CONT;
        into = into.subspan(4);
    } else {
        assert(false);
    }
}

void utf8push(std::string& str, Codepoint cp) {
    if (cp < 0x80) {
        str.push_back(uint8_t(cp));
    } else if (cp < 0x800) {
        str.push_back(uint8_t(cp >> 6U) | UTF8LEAD2);
        str.push_back(uint8_t(cp & 63U) | UTF8CONT);
    } else if (cp < 0x10000) {
        str.push_back(uint8_t(cp >> 12U) | UTF8LEAD3);
        str.push_back(uint8_t((cp >> 6U) & 63U) | UTF8CONT);
        str.push_back(uint8_t(cp & 63U) | UTF8CONT);
    } else if (cp <= MAX_UNICODE_CODEPOINT) {
        str.push_back(uint8_t(cp >> 18U) | UTF8LEAD4);
        str.push_back(uint8_t((cp >> 12U) & 63U) | UTF8CONT);
        str.push_back(uint8_t((cp >> 6U) & 63U) | UTF8CONT);
        str.push_back(uint8_t(cp & 63U) | UTF8CONT);
    } else {
        str.push_back(0);  // questionable
    }
}

void SaveCodepointEscLong(std::string& into, Codepoint cp) {
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
            case 27:
            case 28:
            case 29:
            case 30:
            case 31:
                into.push_back(ASCII::BSLASH);
                into.push_back('u');
                into.push_back('0');
                into.push_back('0');
                into.push_back(BASE_PUNCT[cp >> 4]);
                into.push_back(BASE_PUNCT[cp & 0xf]);
                break;
            case ASCII::NUL:
                into.push_back(ASCII::BSLASH);
                into.push_back('0');
                break;
            case ASCII::BS:
                into.push_back(ASCII::BSLASH);
                into.push_back('b');
                break;
            case ASCII::HT:
                into.push_back(ASCII::BSLASH);
                into.push_back('t');
                break;
            case ASCII::LF:
                into.push_back(ASCII::BSLASH);
                into.push_back('n');
                break;
            case ASCII::CR:
                into.push_back(ASCII::BSLASH);
                into.push_back('r');
                break;
            case ASCII::FF:
                into.push_back(ASCII::BSLASH);
                into.push_back('f');
                break;
            case ASCII::BEL:
                into.push_back(ASCII::BSLASH);
                into.push_back('a');
                break;
            case ASCII::VT:
                into.push_back(ASCII::BSLASH);
                into.push_back('v');
                break;
            case ASCII::QUOTE:
                into.push_back(ASCII::BSLASH);
                into.push_back(ASCII::QUOTE);
                break;
            case ASCII::BSLASH:
                into.push_back(ASCII::BSLASH);
                into.push_back(ASCII::BSLASH);
                break;
            default:
                into.push_back(uchar8(cp));
                break;
        }
    } else if (cp < 0x800) {
        into.push_back(uchar8(cp >> 6) | UTF8LEAD2);
        into.push_back(uchar8(cp & 63) | UTF8CONT);
    } else if (cp < 0x10000) {
        into.push_back(uchar8(cp >> 12) | UTF8LEAD3);
        into.push_back(uchar8((cp >> 6) & 63) | UTF8CONT);
        into.push_back(uchar8(cp & 63) | UTF8CONT);
    } else if (cp <= MAX_UNICODE_CODEPOINT) {
        into.push_back(uchar8(cp >> 18) | UTF8LEAD4);
        into.push_back(uchar8((cp >> 12) & 63) | UTF8CONT);
        into.push_back(uchar8((cp >> 6) & 63) | UTF8CONT);
        into.push_back(uchar8(cp & 63) | UTF8CONT);
    } else {
        assert(false);
    }
}

}  // namespace ron
