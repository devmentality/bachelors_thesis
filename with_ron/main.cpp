#include "ron/ron.hpp"

using namespace ron;

PROC StreamAPIExamples() {
    ron::Op op{Uuid{"0-0-B"}, Uuid{"yarn"}};
    op.Values().PushUtf8String("hello from my proj!");
    RONtStream std{};
    CALL(std.Open("-", Stream::WRITE));  // open stdout
    CALL(std.FeedOp(op));                // serialize an op
    CALL(std.FeedEOF());                 // terminate the RON frame
    CALL(std.Drain());                   // send the data to the console
    
    DONE;
}

PROC TestAll() {
    CALL(StreamAPIExamples());
    DONE;
}

int main(int argn, char** args) {
    auto status = TestAll();
    if (status != OK) Report(status);
    return status._s;
}
