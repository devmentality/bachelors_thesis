#include <iostream>

#include "../ron.hpp"

using namespace ron;
using namespace std;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    // 0 RONv -> 1 RONc -> 2 RONv -> 3 RONc,  -> 4 RONt -> 5 RONc
    RONvStream ronv01{};
    RONtStream ront4{};
    RONvStream writer12{};
    RONtStream writer14{};
    assert(OK== ronv01.Create());
    assert(OK==ront4.Create());
    assert(OK==writer12.Create());
    assert(OK==writer14.Create());
    ronv01.FeedSpan(span8c{Data,Size});
    Ops ronc{};
    ronc.emplace_back();
    while (OK == ronv01.DrainOp(ronc.back())) { // TODO captive op
        assert(OK == writer12.FeedOp(ronc.back()));
        assert(OK == writer14.FeedOp(ronc.back()));
        ronc.emplace_back();
    }
    ronc.pop_back();
    if (ronc.empty()) {
        return 0;
    }
    assert(OK==writer12.FeedEOF());
    assert(OK==writer14.FeedEOF());
    Op op{};
    RONvStream reader23{};
    reader23.Create();
    auto i = ronc.begin();
    while (!writer12.Filled().empty()) {
        assert(OK == reader23.FeedAllFromStream(writer12, 4));
        while (OK == reader23.DrainOp(op)) {
            assert(i < ronc.end());
            if (!op.IsEqual(*i)) {  // 1==3 ?
                assert(false);
            }
            ++i;
        }
    }
    assert(i == ronc.end());
    // TODO writer14, RONt
    return 0;
}
