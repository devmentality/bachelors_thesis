//
// Created by gritzko on 4/12/21.
//
#include <iostream>

#include "../ron.hpp"

using namespace ron;
using namespace std;

int TCTC(span8 data) {
    // 0 RONt --> 1 RONc --> 2 RONt --> 3 RONc
    RONtStream t01{Stream::CLOSED, Stream::TMP};
    t01.FeedSpan(data);
    RONtStream t12{Stream::CLOSED, Stream::TMP};
    Op op{};
    Ops ronc{};
    WHILE(loads, t01.DrainOp(op)) {
        assert(OK==t12.FeedOp(op));
        ronc.push_back(op);
    }
    if (ronc.empty()) return 0;
    t12.FeedEOF();
    RONtStream t23{Stream::CLOSED, Stream::TMP};
    t23.FeedSpan(t12.Filled());
    auto i = ronc.begin();
    WHILE(moreloads, t23.DrainOp(op)) {
        assert(i < ronc.end());
        assert(op.IsEqual(*i));
        ++i;
    }
    assert(i == ronc.end());

    return 0;
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    return TCTC(span8{(uint8_t *)Data, Size});
}
