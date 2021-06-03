#ifndef ron_const_hpp
#define ron_const_hpp
#include <assert.h>
#include <sys/errno.h>

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include "bits.hpp"

namespace ron {

using uchar8 = unsigned char;
using uchar8_ptr = uchar8 const*;

/** Frame size or any other size limited by frame size (e.g. string length) */
using FSize = uint32_t;
#define FSIZE(s) static_cast<FSize>(s)

/** Max RON frame size is 1<<30 (a frame is atomically processed, so 1GB max) */
constexpr FSize MAX_FRAME_SIZE{1U << 30U};
constexpr FSize FSIZE_BITS{MAX_FRAME_SIZE - 1};

using Index = uint32_t;
using IndexPair = std::pair<Index, Index>;
using Word = uint64_t;
using Double = std::pair<Word, Word>;
using Codepoint = uint32_t;
using Float = double;
using Integer = int64_t;

using Words = std::vector<Word>;
using Indices = std::vector<Index>;

constexpr Word MAX_WORD{(1UL << 60) - 1};
constexpr Word WORD_FLAG_BITS{15UL << 60U};

enum RON { RONc, RONv, RONt };

enum ASCII : uint8_t {
    NUL = 0,
    SOH = 1,
    STX = 2,
    ETX = 3,
    EOT = 4,
    ENQ = 5,
    ACK = 6,
    BEL = 7,
    BS = '\b',
    HT = '\t',
    TAB = '\t',
    LF = '\n',
    VT = 11,
    FF = 12,
    CR = 13,
    SO = 14,
    SI = 15,
    DLE = 16,
    DC1 = 17,
    DC2 = 18,
    DC3 = 19,
    DC4 = 20,
    NAK = 21,
    SYN = 22,
    ETB = 23,
    CAN = 24,
    EM = 25,
    SUB = 26,
    ESC = 27,
    FS = 28,
    GS = 29,
    RS = 30,
    US = 31,
    SPACE = 32,
    BSLASH = '\\',
    QUOTE = '\'',
    SLASH = '/',
    DBLQUOTE = '"'
};

// UUID_PUNCT has Textual RON punctuation for RON UUID versions
constexpr char UUID_PUNCT[] = " ^+-";

enum UUID : uint8_t {
    /** Global names; up to 20 Base64 chars, e.g. "test" */
    NAME = 0,
    /** Replica-local names, 10 chars, e.g. "test^replicaId" */
    TAG = 1,
    /** Logical timestamps, counter+replica, e.g. "123+replicaId" */
    LAMPORT = 2,
    /** Object/Yarn log(ical) timestamps, e.g. "1-2-replicaId" */
    YOLO = 3
};
constexpr uint64_t NAME_FLAGS = uint64_t(NAME) << 60;
constexpr uint64_t TAG_FLAGS = uint64_t(TAG) << 60;
constexpr uint64_t LAMPORT_FLAGS = uint64_t(LAMPORT) << 60;
constexpr uint64_t YOLO_FLAGS = uint64_t(YOLO) << 60;

// AtomPunct is Textual RON punctuation for value atoms
constexpr char ATOM_PUNCT[] = ">='^";
constexpr uint8_t STRING_QUOTE{'\''};

// ATOM is a enum of atom types; stored in the upper 2 bits of the origin
enum ATOM : uint8_t {
    // ID is an UUID atom (kind of a pointer)
    ID = 0,
    // INT is int64
    INT = 1,
    // STRING is an Unicode string
    STRING = 2,
    // FLOAT is a 64-bit ISO float
    FLOAT = 3,
    //
    ANY = 4,
};

inline uint64_t flags(ATOM type) { return uint64_t(type) << 62; }

// TERM_PUNCT has Textual RON punctuation for ops
constexpr char TERM_PUNCT[] = ",;!?";
constexpr uint8_t FRAME_PART_PUNCT{','};

constexpr char ID_PUNCT = '@';
constexpr char REF_PUNCT = ':';

constexpr char SPAN_BRACKET_OPEN = '(';
constexpr char SPAN_BRACKET_CLOSE = ')';

enum TERM : uint8_t { FACT = 1, PART = 0, ARTEFACT = 2, QUERY = 3 };

// BASE_PUNCT is Base64x64, the only Base64 variant that sorts properly.
constexpr char BASE_PUNCT[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz~";
constexpr char HEX_PUNCT[] = "0123456789ABCDEF";
constexpr char HEX_PUNCT_LOWERCASE[] = "0123456789abcdef";

enum BASE64_DIGITS : uint8_t {
    BASE_0,
    BASE_1,
    BASE_2,
    BASE_3,
    BASE_4,
    BASE_5,
    BASE_6,
    BASE_7,
    BASE_8,
    BASE_9,
    BASE_A,
    BASE_B,
    BASE_C,
    BASE_D,
    BASE_E,
    BASE_F,
    BASE_G,
    BASE_H,
    BASE_I,
    BASE_J,
    BASE_K,
    BASE_L,
    BASE_M,
    BASE_N,
    BASE_O,
    BASE_P,
    BASE_Q,
    BASE_R,
    BASE_S,
    BASE_T,
    BASE_U,
    BASE_V,
    BASE_W,
    BASE_X,
    BASE_Y,
    BASE_Z,
    BASE_UNDER,
    BASE_a,
    BASE_b,
    BASE_c,
    BASE_d,
    BASE_e,
    BASE_f,
    BASE_g,
    BASE_h,
    BASE_i,
    BASE_j,
    BASE_k,
    BASE_l,
    BASE_m,
    BASE_n,
    BASE_o,
    BASE_p,
    BASE_q,
    BASE_r,
    BASE_s,
    BASE_t,
    BASE_u,
    BASE_v,
    BASE_w,
    BASE_x,
    BASE_y,
    BASE_z,
    BASE_TILDE
};

constexpr unsigned INDEX64LEN = 5;
constexpr unsigned WORD64LEN = 10;

// FLOAT10LEN is the max length of a decimal FLOAT
constexpr unsigned FLOAT10LEN = 24;

// RON frame size limit is 1GB, for the reason we can't demand every
// implementation to atomically process bigger pieces of data. Hence,
// all sizes are limited by 2^30
constexpr FSize MAX_SIZE = (1U << 30U) - 1;
constexpr FSize INVALID_SIZE = MAX_SIZE + 1;

// MAX_CP is the highest Unicode codepoint.
constexpr unsigned MAX_UNICODE_CODEPOINT = 0x10FFFF;
constexpr unsigned INVALID_CODEPOINT = 1U << 30U;

// MAX_SPAN_SIZE is the max length of an op span;
// 1024 is good enough in terms of compression rates, safe enough in terms of
// DDoS opportunities
constexpr FSize MAX_SPAN_SIZE = 64;

constexpr unsigned INDEX_BIT_SIZE = 32U;
constexpr unsigned INDEX_PAYLOAD_BIT_SIZE = 30U;
constexpr Index INDEX_PAYLOAD_BITS = (1U << INDEX_PAYLOAD_BIT_SIZE) - 1U;
// yarns and objects are capped at ~1 billion ops (1^30)
constexpr Index MAX_INDEX = (1U << INDEX_PAYLOAD_BIT_SIZE) - 1U;
constexpr Index INVALID_INDEX = MAX_INDEX + 1;
// words are capped at 60 bits payload (4 bits for flags, etc)
constexpr unsigned WORD_PAYLOAD_BIT_SIZE = 60;
constexpr uint64_t WORD_PAYLOAD_BITS = (1UL << WORD_PAYLOAD_BIT_SIZE) - 1;
static constexpr Word RAM_PAGE_SIZE = 1UL << 12U;
/** sector writes are believed to be atomic */
static constexpr Index DISK_SECTOR_SIZE = 512;
static constexpr Word RAM_CACHE_LINE = 1UL << 6U;
static constexpr Index MAX_BASE64_WORD_SIZE = 10;
static constexpr Index MAX_BASE64_INDEX_SIZE = 5;
constexpr Index MAX_RONc_OP_BYTE_SIZE = MAX_INDEX + 1;  // 1GB
/** this is the max string size that may still fit into 1GB RONc op */
constexpr Index MAX_RONc_STRING_CODEPOINT_SIZE =
    (MAX_RONc_OP_BYTE_SIZE - 8 * 6) >> 2U;

/** a position in a file, e.g. RON oplog */
using Position = uint64_t;

static constexpr Position MAX_POSITION = WORD_PAYLOAD_BITS;
static constexpr Position INVALID_POSITION = MAX_POSITION + 1;

/** Status conveys subroutine success/failure/other condition.
 * A status is a 64 bit int, human readable in Base64x64.
 * No exceptions here because:
 * (1) C++ exceptions are not universally used. As this is a library,
 *      there is a probability it has to be used by an exception-free
 *      codebase.
 * (2) Statuses are more explicit, predictable, performant.
 * (3) Subroutines may return *useful* codes other than OK,
 *     e.g. NOTFOUND or FAILNOYARN or whatever.
 * (4) Statuses are formal enough, delivered over the network if
 *     necessary. Like a mix of HTTP codes and C return codes.
 *
 * Overall, this approach was inspired by the Google's C++ dialect
 * (leveldb, abseil). Plus, it had to be network- and RON-friendly.
 */
struct Status {
    Word _s;
    bool operator==(Status b) const { return _s == b._s; }
    bool operator==(Word w) const { return _s == w; }
    bool operator!=(Status b) const { return _s != b._s; }
    Status& operator=(Status b) {
        _s = b._s;
        return *this;
    }
    Status TrimExt() const { return Status{_s & (~63UL)}; }
    Status Ext(int ext) const { return Status{_s + ext}; }
    uint8_t Ext() const { return _s & 63U; }
    bool Is(Status trimmed) const { return TrimExt() == trimmed; }
    bool Ok() const { return _s == 0; }
    inline std::string String() const;
};
constexpr Status OK{0UL};
constexpr Status NOTOK{421215779250241536UL};

constexpr Word DOUBLE_TILDE{1152640029630136320UL};
constexpr Word TILDE{1134907106097364992UL};

inline Word DoubleTildeOf(Word name) { return (name >> 12U) | DOUBLE_TILDE; }

inline bool IsDoubleTilde(Word name) {
    return (name & DOUBLE_TILDE) == DOUBLE_TILDE;
}

inline Word TildeOf(Word name) { return (name >> 6U) | TILDE; }

inline bool IsTilde(Word name) { return (name & TILDE) == TILDE; }

inline Word RandomWord() {
    return ((Word(random()) << 32U) | random()) & WORD_PAYLOAD_BITS;
}

/** Aborts the program if the condition does not hold (contract violation, bug,
 *  the situation was not planned for). Differently from assert(), works in
 *  release builds. */
#define MUST(condition, comment)                                           \
    {                                                                      \
        if (!(condition)) {                                                \
            fprintf(stderr, "contract violation at %s:%d, %s\n", __FILE__, \
                    __LINE__, comment);                                    \
            abort();                                                       \
        }                                                                  \
    }

#define MUST_OK(statement, comment)                                 \
    {                                                               \
        auto __s = (statement);                                     \
        if (__s != OK) {                                            \
            std::string __err = __s.String();                       \
            fprintf(stderr, "contract violation %s at %s:%d, %s\n", \
                    __err.c_str(), __FILE__, __LINE__, comment);    \
            abort();                                                \
        }                                                           \
    }

/** A function returns a value, never fails. A procedure may fail (returns
 * Status). A function is entity-named, a proc is verb-named. Usage:
 * `PROC DoSomething(int &return_value, int parameter);`
 * `CALL(DoSomething(x,y));` */
#define PROC Status

/** Invokes a PROC; needs OK to continue executing; returns if not OK. */
#define CALL(stmt)                 \
    {                              \
        PROC __s = (stmt);         \
        if (unlikely(__s != OK)) { \
            return __s;            \
        }                          \
    }

#define TRY(label, stmt) Status label = (stmt);

#define WHILE(label, stmt) \
    PROC label{};          \
    while (((label) = (stmt)).Ok())

#define FOR(label, init_stmt, next_stmt) \
    PROC label = (init_stmt);            \
    for (; (label).Ok(); (label) = (next_stmt))

#define SKIP(stmt)                       \
    {                                    \
        PROC __s = (stmt);               \
        if (likely(__s == OK)) continue; \
    }

#define DONE \
    { return OK; }

#define FAIL(code) \
    { return code; }

#define NEED_EOF(stmt)                      \
    {                                       \
        PROC __s = (stmt);                  \
        if (unlikely(!__s.Is(ENDOFRAME))) { \
            return __s;                     \
        }                                   \
    }

/** Need OK or NOTFOUND to continue executing the function. */
#define CALL_OKNF(stmt)                               \
    {                                                 \
        PROC __s = (stmt);                            \
        if (__s != OK && __s.TrimExt() != NOTFOUND) { \
            return __s;                               \
        }                                             \
    }

/** Need OK or NOTFOUND to continue executing the function. */
#define NEED_OKEOF(stmt)                               \
    {                                                  \
        PROC __s = (stmt);                             \
        if (__s != OK && __s.TrimExt() != ENDOFRAME) { \
            return __s;                                \
        }                                              \
    }

/** Need OK or ENDOFRAME to continue executing the function. */
#define CALL_OKEOF(stmt)                       \
    {                                          \
        PROC __s = (stmt);                     \
        if (__s != OK && !__s.Is(ENDOFRAME)) { \
            return __s;                        \
        }                                      \
    }

/** Need OK or NOTFOUND to continue executing the function. */
#define CALL_OKOR(stmt)                        \
    {                                          \
        PROC __s = (stmt);                     \
        if (__s != OK && !__s.Is(OUTORANGE)) { \
            return __s;                        \
        }                                      \
    }

/** Need this condition to hold to continue executing the function. */
#define NEED(cond, fail) \
    if (!likely(cond)) { \
        return fail;     \
    }
#define NEED_OK(cond)           \
    if (!likely((cond).Ok())) { \
        return cond;            \
    }
#define NEED_ND(cond)           \
    if (!likely(cond==NODATA)) { \
        return cond;            \
    }
#define NEED_NF(stmt)                      \
    {                                      \
        PROC __s = (stmt);                 \
        if (!__s.Is(NOTFOUND)) return __s; \
    }
#define NEED_OKNF(stmt)                                 \
    {                                                   \
        PROC __s = (stmt);                              \
        if (__s != OK && !__s.Is(NOTFOUND)) return __s; \
    }

/** Need the condition to hold to continue executing the function.
 *  If not, prints the warning based on the errno, returns. */
#define CALL_C(cond, fail)                                                     \
    if (!likely(cond)) {                                                       \
        fprintf(stderr, "%s at %s:%d\n", strerror(errno), __FILE__, __LINE__); \
        return (fail).Ext(errno);                                              \
    }

#define CALL_C_MSG(cond, fail, msg)                                          \
    if (!likely(cond)) {                                                     \
        fprintf(stderr, "%s, %s at %s:%d\n", strerror(errno), msg, __FILE__, \
                __LINE__);                                                   \
        return (fail).Ext(errno);                                            \
    }

/** checks std::error_code; please use no extension codes (uses the checked one)
 */
#define CALL_CPP(err, code)                        \
    {                                              \
        if (err) return (code).Ext((err).value()); \
    }

/**
 * Statuses / return codes
 * Severity is according to the convention:
 *
 *   NO*   normal situation, can't do something (e.g. NODATA)
 *   BAD*  bad argument, bad situation, bad input data
 *   FAIL* external failure, some call did not work
 *   END*  end of the input or suchlike
 *   IS*   not an error or warning, something is...
 *
 * */
#include "status.hpp"

static constexpr Codepoint EMPTY_STRING32[1]{};
inline const Codepoint* ErrorMessage(Status code) {
    for (auto& e : ERRORS)
        if (e.first == code) return (Codepoint*)e.second;
    return EMPTY_STRING32;
}

constexpr Word TEST{1020592531424935936};
constexpr Word NEVER{63UL << 54};
static constexpr IndexPair INVALID_INDEX_PAIR{INVALID_INDEX, INVALID_INDEX};

// parser lookup table (reverse Base64x64 + punctuation)
constexpr uint8_t ABC[] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 2,   255, 1,   0,   1,   255, 2,   0,   5,   0,   2,   0,
    3,   0,   255, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   3,   1,
    255, 1,   0,   3,   2,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
    20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,
    35,  1,   255, 4,   3,   36,  0,   37,  38,  39,  40,  41,  42,  43,  44,
    45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
    60,  61,  62,  2,   255, 3,   63,  255};

// reverse hex lookup table
const int8_t ABC16[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1};

const int8_t ABC64[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1,
    36, -1, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
    54, 55, 56, 57, 58, 59, 60, 61, 62, -1, -1, -1, 63, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1};

union Halves {
    uint64_t word;
    struct {
        /** least/most significant halves of the word */
        uint32_t ls_half, ms_half;
    };
    static constexpr Word LOWER_INC = 1UL;
    static constexpr Word UPPER_INC = 1UL << 32U;
    static constexpr Word DOUBLE_INC = LOWER_INC | UPPER_INC;
    Halves() = default;
    inline Halves(uint32_t ls, uint32_t ms) : ls_half{ls}, ms_half{ms} {}
    explicit Halves(uint64_t w) : word{w} {}
    static Halves& Of(Word& w) { return reinterpret_cast<Halves&>(w); }
    static const Halves& Of(const Word& w) {
        return reinterpret_cast<const Halves&>(w);
    }
    bool operator==(Halves b) const { return word == b.word; }
};

inline Word& as_word(Float& f) { return reinterpret_cast<Word&>(f); }

inline Word WordMix(Word x) {
    x = (x ^ (x >> 30U)) * UINT64_C(0xbf58476d1ce4e5b9);
    x = (x ^ (x >> 27U)) * UINT64_C(0x94d049bb133111eb);
    x = x ^ (x >> 31U);
    return x;
}

inline Index PaddedStringSize(Index cp_size) { return cp_size + (cp_size & 1); }

inline Codepoint* StringAlloc(Index cp_size) {
    auto size = PaddedStringSize(cp_size);
    auto ret = (Codepoint*)malloc(size << 2U);
    ret[size - 1] = 0;  // ensure valid padding
    return ret;
}

inline void StringCopy(Codepoint* to, const Codepoint* from, Index cp_size) {
    memcpy(to, from, PaddedStringSize(cp_size) << 2U);
}

/** @return -1 less, 0 equal, 1 more */
inline int StringCompare(const Codepoint* a, const Codepoint* b,
                         Index cp_size) {
    auto word_size = PaddedStringSize(cp_size) >> 1U;
    auto aw = (Word*)a;
    auto bw = (Word*)b;
    Index i = 0;
    while (i < word_size) {
        if (aw[i] != bw[i]) return aw[i] < bw[i] ? -1 : 1;
        ++i;
    }
    return 0;
}

inline void ToLower(std::string& str) {
    for (auto& c : str)
        if (c >= 'A' && c <= 'Z') c += 'a' - 'A';
}

inline bool IsPowerOfTwo(Word w) { return 0==((w - 1U) & w); }

using IndexRange = std::pair<Index, Index>;

}  // namespace ron

#endif
