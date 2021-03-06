// Copyright 2008 Google Inc.  All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//* Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//* Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//* Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Code generated by the Protocol Buffer compiler is owned by the owner
// of the input file used when generating it.  This code is not
// standalone and requires a support library to be linked with it.  This
// support library is itself covered by the above license.

#ifndef protobuf_h
#define protobuf_h
#include <assert.h>

#include <cstdint>

namespace ron {

#ifdef __GNUC__
// Provided at least since GCC 3.0.
#define PROTOBUF_PREDICT_FALSE(x) (__builtin_expect(x, 0))
#define PROTOBUF_PREDICT_TRUE(x) (__builtin_expect(!!(x), 1))
#else
#define PROTOBUF_PREDICT_FALSE(x) (x)
#define PROTOBUF_PREDICT_TRUE(x) (x)
#endif

template <typename T>
inline static uint8_t UnsafeVarint(T value, uint8_t *ptr) {
    static_assert(std::is_unsigned<T>::value,
                  "Varint serialization must be unsigned");
    if (value < 0x80) {
        ptr[0] = static_cast<uint8_t>(value);
        return 1;
    }
    ptr[0] = static_cast<uint8_t>(value | 0x80);
    value >>= 7;
    if (value < 0x80) {
        ptr[1] = static_cast<uint8_t>(value);
        return 2;
    }
    auto p0 = ptr;
    ptr++;
    do {
        *ptr = static_cast<uint8_t>(value | 0x80);
        value >>= 7;
        ++ptr;
    } while (PROTOBUF_PREDICT_FALSE(value >= 0x80));
    *ptr++ = static_cast<uint8_t>(value);
    return ptr - p0;
}

template <int N>
void DecodeVarint64KnownSize(const uint8_t *buffer, uint64_t &value) {
    uint64_t result = static_cast<uint64_t>(buffer[N - 1]) << (7 * (N - 1));
    for (int i = 0, offset = 0; i < N - 1; i++, offset += 7) {
        result += static_cast<uint64_t>(buffer[i] - 0x80) << offset;
    }
    value = result;
}

inline uint8_t ReadVarint64FromArray(const uint8_t *buffer, uint64_t &value) {
    // Assumes varint64 is at least 2 bytes.
    assert(buffer[0] >= 128);

    if (buffer[1] < 128) {
        DecodeVarint64KnownSize<2>(buffer, value);
        return 2;
    } else if (buffer[2] < 128) {
        DecodeVarint64KnownSize<3>(buffer, value);
        return 3;
    } else if (buffer[3] < 128) {
        DecodeVarint64KnownSize<4>(buffer, value);
        return 4;
    } else if (buffer[4] < 128) {
        DecodeVarint64KnownSize<5>(buffer, value);
        return 5;
    } else if (buffer[5] < 128) {
        DecodeVarint64KnownSize<6>(buffer, value);
        return 6;
    } else if (buffer[6] < 128) {
        DecodeVarint64KnownSize<7>(buffer, value);
        return 7;
    } else if (buffer[7] < 128) {
        DecodeVarint64KnownSize<8>(buffer, value);
        return 8;
    } else if (buffer[8] < 128) {
        DecodeVarint64KnownSize<9>(buffer, value);
        return 9;
    } else if (buffer[9] < 128) {
        DecodeVarint64KnownSize<10>(buffer, value);
        return 10;
    } else {
        // We have overrun the maximum size of a varint (10 bytes). Assume
        // the data is corrupt.
        return 0;
    }
}

inline uint8_t ReadVarint64(const uint8_t *buf, uint64_t &value) {
    if (PROTOBUF_PREDICT_TRUE(*buf < 0x80)) {
        value = *buf;
        return 1;
    }
    return ReadVarint64FromArray(buf, value);
}

constexpr int kMaxVarintBytes = 10;
constexpr int kMaxVarint32Bytes = 5;

inline uint8_t ReadVarint32FromArray(const uint8_t *buffer, uint32_t &value) {
    assert(*buffer >= 128);
    const uint8_t *ptr = buffer;
    uint32_t b;
    uint32_t result = *ptr - 0x80;
    ++ptr;  // We just processed the first byte.  Move on to the second.
    b = *(ptr++);
    result += b << 7;
    if (!(b & 0x80)) goto done;
    result -= 0x80 << 7;
    b = *(ptr++);
    result += b << 14;
    if (!(b & 0x80)) goto done;
    result -= 0x80 << 14;
    b = *(ptr++);
    result += b << 21;
    if (!(b & 0x80)) goto done;
    result -= 0x80 << 21;
    b = *(ptr++);
    result += b << 28;
    if (!(b & 0x80)) goto done;
    // "result -= 0x80 << 28" is irrevelant.

    // If the input is larger than 32 bits, we consider the data corrupt!
    return 0;

done:
    value = result;
    return ptr - buffer;
}

inline uint8_t ReadVarint32(const uint8_t *buf, uint32_t &value) {
    if (PROTOBUF_PREDICT_TRUE(*buf < 0x80)) {
        value = *buf;
        return 1;
    }
    return ReadVarint32FromArray(buf, value);
}

inline uint64_t ZigZagEncode64(int64_t n) {
    // Note:  the right-shift must be arithmetic
    // Note:  left shift must be unsigned because of overflow
    return (static_cast<uint64_t>(n) << 1) ^ static_cast<uint64_t>(n >> 63);
}

inline int64_t ZigZagDecode64(uint64_t n) {
    // Note:  Using unsigned types prevent undefined behavior
    return static_cast<int64_t>((n >> 1) ^ (~(n & 1) + 1));
}

}  // namespace ron

#endif /* protobuf_h */
