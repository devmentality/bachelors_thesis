//
// Created by gritzko on 05.09.19.
//

#ifndef PULSAR_UTF8_HPP
#define PULSAR_UTF8_HPP

#include <vector>

#include "bits.hpp"
#include "const.hpp"
#define span_CONFIG_INDEX_TYPE size_t
#define span_CONFIG_NO_EXCEPTIONS 1
#define span_FEATURE_MAKE_SPAN 1
#include "deps/span_pro/span.hpp"

namespace ron {

using string16 = std::vector<char16_t>;
using string32 = std::vector<Codepoint>;
using span32 = nonstd::span<uint32_t>;
using span32c = nonstd::span<const uint32_t>;
using span16 = nonstd::span<uint32_t>;
using span16c = nonstd::span<const uint32_t>;
using span8c = nonstd::span<const uint8_t>;
using span8 = nonstd::span<uint8_t>;
using Strings = std::vector<std::string>;
using span64 = nonstd::span<Word>;
using span64c = nonstd::span<const Word>;
using span128 = nonstd::span<Double>;

inline span8c as_span8c(const std::string& a) {
    return span8c{(const uint8_t*)a.data(), (size_t)a.size()};
}

inline span32 as_span32(span64 data) {
    return span32{(uint32_t*)data.data(), data.size()<<1U};
}

inline span32c as_span32c(span64c data) {
    return span32c{(const uint32_t* const)data.data(), data.size()<<1U};
}

inline span32c as_span32c(const Codepoint* str) {
    size_t l{};
    while (str[l]) ++l;
    return span32c{(uint32_t*)str, l};
}

inline span32c as_span32c(const string32& str) {
    return span32c{(uint32_t*)str.data(), str.size()};
}

inline std::string as_string(span8c::pointer a, span8c::pointer b) {
    return std::string{(const char*)a, (const char*)b};
}

inline span8 as_span8(std::string& a) {
    return span8{(uint8_t*)a.data(), (size_t)a.size()};
}

inline std::string as_string(span8c data) {
    return std::string{(char*)data.data(), data.size()};
}

inline span8c as_span8c(const char* data) {
    return span8c{(const uint8_t*)data, strlen(data)};
}

inline bool IsAllSpaces(span8c t) {
    for (auto c : t)
        if (c != ' ') return false;
    return true;
}

static const uint8_t UTF8JUMPS[16] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 3, 4,
};

constexpr uint8_t UTF8CONT = 128;
constexpr uint8_t UTF8LEAD2 = 128 | 64;
constexpr uint8_t UTF8LEAD3 = 128 | 64 | 32;
constexpr uint8_t UTF8LEAD4 = 128 | 64 | 32 | 16;

constexpr unsigned MAX_UTF8_CP_SIZE = 5;

void utf8push(std::string& str, Codepoint cp);
bool is_utf8(const std::string& str);
std::size_t utf8size(const std::string& str);

inline bool is_utf8_cont(uint8_t b) { return b >> 6U == 2; }

Index utf8size(span8c span);

PROC ParseUtf8(string32& into, span8c data);
PROC SaveUtf8(std::string& into, span32c data);
PROC SaveUtf8Esc(std::string& into, span32c data);
inline Status SaveUtf8(std::string& into, const string32& data) {
    return SaveUtf8(into, as_span32c(data));
}
inline Status SaveUtf8Esc(std::string& into, const string32& data) {
    return SaveUtf8Esc(into, as_span32c(data));
}
PROC SaveUtf16(string16& into, span32 data);

inline Status ParseUtf8(string32& into, const std::string& data) {
    return ParseUtf8(into, as_span8c(data));
}

void SaveUtf8CodepointLong(span8& into, Codepoint cp);
void SaveUtf8CodepointLong(std::string& into, Codepoint cp);

inline void SaveUtf8Codepoint(span8& into, Codepoint cp) {
    if (cp < 0x80) {
        into.front() = uchar8(cp);
        into = into.subspan(1);
    } else {
        SaveUtf8CodepointLong(into, cp);
    }
}

inline void SaveUtf8Codepoint(std::string& into, Codepoint cp) {
    if (cp < 0x80) {
        into.push_back(uchar8(cp));
    } else {
        SaveUtf8CodepointLong(into, cp);
    }
}

void SaveCodepointEscLong(std::string& into, Codepoint cp);

inline void SaveCodepointEsc(std::string& into, Codepoint cp) {
    if (cp < 63 && cp >= 32 && cp != '\'' && cp != '\\') {
        into.push_back(cp);
    } else {
        SaveCodepointEscLong(into, cp);
    }
}

}  // namespace ron

#endif  // PULSAR_UTF8_HPP
