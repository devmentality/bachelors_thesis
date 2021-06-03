//
//  uuid.cpp
//  pulsar
//
//  Created by gritzko gritzko on 01/09/2019.
//  Copyright © 2019 gritzko. All rights reserved.
//

#include "uuid.hpp"

namespace ron {

PROC Uuid::ParseWord(Word& value, span8c data) {
    NEED(!data.empty() && data.size() <= MAX_BASE64_WORD_SIZE, BADSYNTAXU);
    auto f = data.begin();
    value = 0;
    uint32_t shift{60};
    for (auto i = f; i < data.end(); ++i) {
        value <<= 6U;
        value |= ABC64[*i];
        shift -= 6U;
    }
    value <<= shift;
    DONE;
}

PROC Uuid::ParseDouble(Double& value, span8c data) {
    NEED(data.size() <= MAX_BASE64_WORD_SIZE * 2, BADSYNTAXU);
    if (data.size() > 10) {
        CALL(ParseWord(value.first, data.subspan(0, MAX_BASE64_WORD_SIZE)));
        CALL(ParseWord(value.second, data.subspan(MAX_BASE64_WORD_SIZE)));
    } else {
        CALL(ParseWord(value.first, data));
        value.second = 0;
    }
    DONE;
}

PROC Uuid::Save(span8& into, Uuid value) {
    NEED(into.size() >= MAX_BASE64_SIZE, BADARGMNTt);
    switch (value.Version()) {
        case YOLO:
            SaveIndex(into, value.HomeIndex());
            into.front() = UUID_PUNCT[YOLO];
            into = into.subspan(1);
            SaveIndex(into, value.YarnIndex());
            into.front() = UUID_PUNCT[YOLO];
            into = into.subspan(1);
            SaveWord(into, value.second);
            DONE;
        case LAMPORT:
            SaveWord(into, value.first);
            into.front() = UUID_PUNCT[LAMPORT];
            into = into.subspan(1);
            SaveWord(into, value.second);
            DONE;
        case NAME:
            if (value.second == 0) return SaveWord(into, value.first);
            SavePaddedWord(into, value.first);
            SaveWord(into, value.second);
            DONE;
        case TAG:
            SaveWord(into, value.first);
            into.front() = UUID_PUNCT[TAG];
            into = into.subspan(1);
            SaveWord(into, value.second);
            DONE;
        default:
            MUST(false, "malformed UUID");
    }
}

Uuid Uuid::Time() {
    auto t = time(nullptr);
    struct tm* c = gmtime(&t);
    Uuid ret{};
    auto& B = BASE_PUNCT;
    char ds[21];
    sprintf(ds, "%04d%02d%02d0%02d%02d%02d", c->tm_year + 1900, c->tm_mon,
            c->tm_mday, c->tm_hour, c->tm_min, c->tm_sec);
    auto i = 0;
    while (i < 10) {
        ret.first <<= 6;
        ret.first |= ds[i] - '0';
        ++i;
    }
    ret.first |= 63UL << 6;  // the tilde
    while (i < 15) {
        ret.second <<= 6;
        ret.second |= ds[i] - '0';
        ++i;
    }
    ret.second <<= 30;  // TODO nanoseconds
    return ret;
}

bool Uuid::IsAmbiguousAsNumber(Word value) {
    value <<= 4U;  // flags, varieties
    while (value) {
        uint8_t num = BASE_PUNCT[0x3fU & (value >> 58U)];
        value <<= 6U;
        if (num <= '9') continue;
        if (num == 'e') continue;
        if (num == 'E') continue;
        return false;
    }
    return true;
}

};  // namespace ron
