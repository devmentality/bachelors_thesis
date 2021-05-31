#include <map>
#include <vector>
#include "ron/op.hpp"
#include "schema.h"

using namespace ron;
using namespace std;


Tuple GetKeyTuple(const Op& op) {
    auto reader = op.Values().ReadAtoms();
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


pair<Tuple, Tuple> SplitOperation(const Op& op) {
    auto reader = op.Values().ReadAtoms();
    Tuple key;
    Tuple value;

    while(true) {
        Tuple::Box b;
        reader.ReadAtom(b);
        if (b == Uuid{"~"}) break;
        key.PushBox(b);
    }

    while(!reader.IsEmpty()) {
        Tuple::Box b;
        reader.ReadAtom(b);
        value.PushBox(b);
    }

    return {key, value};
}


map<size_t, Op>* BuildPrimaryKeyMapping(const vector<Op> &operations) {
    hash<Tuple> h{};
    auto mapping = new map<size_t, Op>();

    for(auto op: operations) {
        auto pkey = GetKeyTuple(op);
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

// Main Lww logic
void GenerateResultingOperations(
        vector<Operation>& resulting_operations,
        const vector<Op>& log,
        const vector<Op>& new_ops) {
    hash<Tuple> h{};

    auto log_mapping = BuildPrimaryKeyMapping(log);

    for(const auto& op: new_ops) {
        auto op_parts = SplitOperation(op);
        auto op_key = op_parts.first;
        auto op_value = op_parts.second;
        auto key_hash = h(op_key);

        if (log_mapping->find(key_hash) != log_mapping->end()) {
            auto prev_op = (*log_mapping)[key_hash];
            auto prev_op_value = SplitOperation(prev_op).second;

            if (prev_op.ID() > op.ID())
                continue;

            if (!op_value.IsEmpty() && prev_op_value.IsEmpty()) { // insert deleted row
                resulting_operations.emplace_back("insert", op);
            } else if (op_value.IsEmpty() && !prev_op_value.IsEmpty()) { // delete present row
                resulting_operations.emplace_back("delete", op);
            } else if (!op_value.IsEmpty() && !prev_op_value.IsEmpty()) { // conflicting inserts
                throw domain_error("conflicting inserts are not supported for now");
            }
        }
    }

    delete log_mapping;
}
