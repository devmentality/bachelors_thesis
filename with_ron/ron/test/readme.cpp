/**
#   LibRON

Replicated Object Notation is an op-based data format tailored
for data syncronization in a distributed system.  It models data
as a feed of changes (ops) consisting of atoms (UUID, INT,
STRING, FLOAT).  That makes it easier to perform real-time
synchronization, merge of changes, offline word and other such
things.  `libron` is a C++ lib  implementing parsing and
serialization of RON streams.  It supports the text-based
variant (RONt) and the LEB128 varint based variant (RONv).  The
internal representation is RONc (canonical RON, based on
fixed-width little-endian 64-bit words).  For RON Replicated
Data Types (CRDTs), see `libronrdt`; that library assembles the
data structures out of RON ops.  For the RON oplog, see
`libronp`; that library can store and efficiently query RON
data.

(c) Victor Grishchenko, 2019-2021

**/

#include "ron.hpp"

using namespace ron;

/**
##  RON API

RONc tuples are vectors of 64-bit words; INTs and FLOATs take
one word each, an UUIDs takes two words, STRINGs consume a word
per two symbols (UTF-32). To unify scalars (INT,FLOAT) and
vectors (STRINGs), the underlying RONc representation uses
 *boxes*. A box is a sequence of scalars, so a STRING is a box
 and an array of INTs is also a box. A box starts with a
 descriptor, then followed by the content. A RON tuple is thus a
 concatenation of boxes. The ron::Tuple class implements all
 that machinery intenally.

**/

PROC TupleAPIExamples() {
    ron::Tuple tuple{};
    tuple.PushInteger(-12345);
    tuple.PushUtf8String("test");
    /**
    A Tuple can be iterated in terms of atoms:
    **/
    auto atoms = tuple.ReadAtoms();
    assert(atoms.Has(INT));
    Integer onetwothree;
    atoms.ReadInteger(onetwothree);
    std::string test{};
    atoms.ReadUtf8String(test);
    assert(test == "test");
    /**
    It can also be iterated in terms of boxes:
    **/
    auto boxes = tuple.ReadBoxes();
    ron::Tuple::Box box;
    boxes.ReadBox(box);
    assert(box.Type() == INT);
    assert(box.Size() == 1);
    assert(box.IntegerAt(0) == -12345);
    boxes.ReadBox(box);
    assert(box.Type() == STRING);
    assert(box.Size() == 4);
    assert(box.CodepointAt(1) == 'e');
    DONE;
}

/**

A RON op is a RON tuple plus two UUIDs of metadata: identifier
(id) and reference (ref).  The id is the op's own globally
unique identifier. The reference points to an earlier op.  The
reference is the way we point at the place in the data graph
which the op applies to.

ron::Op API is rather straightforward:
**/
PROC OpAPIExamples() {
    Tuple values{};
    values.PushInteger(123);

    // RON UUIDs are backward-compatible with RFC4122 UUIDs
    // these are 128-bit globally unique identifiers
    // RON UUIDs can also be logical (Lamport) timestamps
    Uuid id{"1-2-origin"}, ref{"0-1-origin"};

    Op op{id, ref, values};  // a new op referencing some past op
    assert(op.Ref().String() == "0-1-origin");
    DONE;
}

/**

##  Stream API

The ron::Stream class is the unifying abstraction for different
forms of input/output.  A Stream is essentially a combination of
a memory range and a file descriptor.  Also, it has input/output
watermarks that track the progress of those activities.  In
libron parlance, a stream is "filled" from one side and
"drained" from the other.  For example, a RONt stream may be
filled from a file and drained by the parser.  Or it can be
filled by a RONv serializer and drained into a TCP connection.
Note that RON parsers are resumable, so they can process data
incrementally.  RON serializers are not resumable and may need
more memory to complete an assignment.
**/
PROC StreamAPIExamples() {
    /**
    Using simple streams is rather straightforward:
    **/
    ron::Stream file{};
    CALL(file.Create("test.ron", Stream::WRITE));
    /**
    Note that most of the library's methods return ron::Status
    (everything is `nothrow`).  ron::Status is a 64-bit error code
    internally, the OK value is 0.  There are some macros to reduce
    error re-throwing boilerplate: PROC, CALL, DONE, FAIL.
    (The lib started with Google C++ Code Style, then slightly
     diverged. libron Status is a RON Word, so it is trivially
     serializable in RON. See status.cpp for free-form boilerplate
     error messages; note that your code may add to that list.)
    **/
    CALL(file.FeedString("@~ 'test RON op';\n"));
    CALL(file.DrainAll());
    CALL(file.Close());
    /**
    ron::Stream descendant classes follow the same Feed/Drain
    pattern with complex data.  See VarIntStream for LEB128 varint
    streams or TextStream for text. On top of those, there are RON
    streams: RONtStream and RONvStream.
    **/
    ron::Op op{Uuid{"0-0-A"}, Uuid{"yarn"}};
    op.Values().PushUtf8String("hello world");
    RONtStream std{};
    CALL(std.Open("-", Stream::WRITE));  // open stdout
    CALL(std.FeedOp(op));                // serialize an op
    CALL(std.FeedEOF());                 // terminate the RON frame
    CALL(std.Drain());                   // send the data to the console
     /**
     RON input works in the reverse order:
     **/
    ron::RONtStream input{};
    CALL(input.Open("test.ron", ron::Stream::READ));
    CALL(input.Feed());
    CALL(input.DrainOp(op));
    std::string str{};
    CALL(op.Values().GetUtf8String(str));
    assert(str == "test RON op");
    CALL(input.Close());
    DONE;
}

PROC TestAll() {
    CALL(TupleAPIExamples());
    CALL(OpAPIExamples());
    CALL(StreamAPIExamples());
    DONE;
}

int main(int argn, char** args) {
    auto status = TestAll();
    if (status != OK) Report(status);
    return status._s;
}
