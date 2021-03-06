// https://gist.github.com/panzi/6856583
// "License": Public Domain
// I, Mathias Panzenböck, place this file hereby into the public domain. Use it
// at your own risk for whatever you like. In case there are jurisdictions that
// don't support putting things in the public domain you can also consider it to
// be "dual licensed" under the BSD, MIT and Apache licenses, if you want to.
// This code is trivial anyway. Consider it an example on how to get the endian
// conversion functions on different platforms.

#ifndef PORTABLE_ENDIAN_H__
#define PORTABLE_ENDIAN_H__
#include <stdint.h>

#if (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && \
    !defined(__WINDOWS__)

#define __WINDOWS__

#endif

#if defined(__linux__) || defined(__CYGWIN__)

#include <endian.h>
#define flip64(x) __bswap_64(x)

#elif defined(__APPLE__)

#include <libkern/OSByteOrder.h>

#define htobe16(x) OSSwapHostToBigInt16(x)
#define htole16(x) OSSwapHostToLittleInt16(x)
#define be16toh(x) OSSwapBigToHostInt16(x)
#define le16toh(x) OSSwapLittleToHostInt16(x)

#define htobe32(x) OSSwapHostToBigInt32(x)
#define htole32(x) OSSwapHostToLittleInt32(x)
#define be32toh(x) OSSwapBigToHostInt32(x)
#define le32toh(x) OSSwapLittleToHostInt32(x)

#define htobe64(x) OSSwapHostToBigInt64(x)
#define htole64(x) OSSwapHostToLittleInt64(x)
#define be64toh(x) OSSwapBigToHostInt64(x)
#define le64toh(x) OSSwapLittleToHostInt64(x)

#define flip64(x) OSSwapInt64(x)

#define __BYTE_ORDER BYTE_ORDER
#define __BIG_ENDIAN BIG_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#define __PDP_ENDIAN PDP_ENDIAN

#elif defined(__OpenBSD__)

#include <sys/endian.h>

#elif defined(__NetBSD__) || defined(__FreeBSD__) || defined(__DragonFly__)

#include <sys/endian.h>

#define be16toh(x) betoh16(x)
#define le16toh(x) letoh16(x)

#define be32toh(x) betoh32(x)
#define le32toh(x) letoh32(x)

#define be64toh(x) betoh64(x)
#define le64toh(x) letoh64(x)

#elif defined(__WINDOWS__)

#include <sys/param.h>
#include <winsock2.h>

#if BYTE_ORDER == LITTLE_ENDIAN

#define htobe16(x) htons(x)
#define htole16(x) (x)
#define be16toh(x) ntohs(x)
#define le16toh(x) (x)

#define htobe32(x) htonl(x)
#define htole32(x) (x)
#define be32toh(x) ntohl(x)
#define le32toh(x) (x)

#define htobe64(x) htonll(x)
#define htole64(x) (x)
#define be64toh(x) ntohll(x)
#define le64toh(x) (x)

#elif BYTE_ORDER == BIG_ENDIAN

/* that would be xbox 360 */
#define htobe16(x) (x)
#define htole16(x) __builtin_bswap16(x)
#define be16toh(x) (x)
#define le16toh(x) __builtin_bswap16(x)

#define htobe32(x) (x)
#define htole32(x) __builtin_bswap32(x)
#define be32toh(x) (x)
#define le32toh(x) __builtin_bswap32(x)

#define htobe64(x) (x)
#define htole64(x) __builtin_bswap64(x)
#define be64toh(x) (x)
#define le64toh(x) __builtin_bswap64(x)

#else

#error byte order not supported

#endif

#define __BYTE_ORDER BYTE_ORDER
#define __BIG_ENDIAN BIG_ENDIAN
#define __LITTLE_ENDIAN LITTLE_ENDIAN
#define __PDP_ENDIAN PDP_ENDIAN

#else

#error platform not supported

#endif

enum HALF : uint8_t {
#ifdef __LITTLE_ENDIAN
    MOST_SIGNIFICANT = 1,
    LEAST_SIGNIFICANT = 0,
#elif
    MOST_SIGNIFICANT = 0,
    LEAST_SIGNIFICANT = 1,
#endif
};

#ifdef _MSC_VER
#include <intrin.h>
uint32_t __inline clz64(uint64_t value) {
    DWORD leading_zero = 0;
    _BitScanReverse64(&leading_zero, value);
    return 63 - leading_zero;
}
#elif __GNUC__
#define clz64(x) __builtin_clzll(x)
#define ctz64(x) __builtin_ctzll(x)
#define clz32(x) __builtin_clz(x)
#define ctz32(x) __builtin_ctz(x)
#define popc32(x) __builtin_popcount(x)
#endif

#ifdef __GNUC__
#define unlikely(x) (__builtin_expect(x, 0))
#define likely(x) (__builtin_expect(!!(x), 1))
#else
#define unlikely(x) (x)
#define likely(x) (x)
#endif

inline uint64_t rotr64(uint64_t val, uint8_t len) {
    return (val >> len) | (val << (64U - len));
}

inline uint64_t rotl64(uint64_t val, uint8_t len) {
    return (val << len) | (val >> (64U - len));
}

#endif
