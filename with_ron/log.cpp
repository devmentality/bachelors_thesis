#include <vector>
#include <iostream>
#include <map>
#include "ron/ron-streams.hpp"
#include "ron/const.hpp"

using namespace ron;
using namespace std;


PROC ReadLog(vector<Op>& operations,  const string& file_name) {
    RONtStream file{};
    CALL(file.Open(file_name, Stream::READ));

    while(true) {
        Op op;
        auto status = file.DrainOp(op);
        if (status == ENDOFRAMEE)
            break;

        operations.push_back(op);
    }
    CALL(file.Close());
}

Tuple GetKeyTuple(const Tuple& values) {
    auto reader = values.ReadAtoms();
    Tuple pkey;

    while(true) {
        Tuple::Box b;
        reader.ReadAtom(b);

        if (b == Uuid{"~"})
            break;

        pkey.PushBox(b);
    }

    return pkey;
}

map<size_t, Op>* BuildPrimaryKeyMapping(const vector<Op> &operations) {
    hash<Tuple> h{};
    auto mapping = new map<size_t, Op>();

    for(auto op: operations) {
        auto pkey = GetKeyTuple(op.Values());
        auto key_hash = h(pkey);

        if (mapping->find(key_hash) != mapping->end()) {
            auto prev_op = (*mapping)[key_hash];

            if (prev_op.ID() < op.ID())
                (*mapping)[key_hash] = op;
        } else {
            (*mapping)[key_hash] = op;
        }
    }

    return mapping;
}






