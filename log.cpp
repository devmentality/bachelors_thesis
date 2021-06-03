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


map<Uuid, Op>* BuildUuidMapping(const vector<Op> &operations) {
    auto mapping = new map<Uuid, Op>();

    for(auto op: operations)
        (*mapping)[op.ID()] = op;

    return mapping;
}


void MergeLogs(vector<Op>& applied_ops_out, const vector<Op> &log, const vector<Op>& patch) {
    auto log_mapping = BuildUuidMapping(log);

    for(auto op: patch) {
        if (log_mapping->find(op.ID()) == log_mapping->end())
            applied_ops_out.push_back(op);
    }

    delete log_mapping;
}


PROC SerializeToRon(const string& log_file_name, const vector<Op>& operations) {
    RONtStream file{};
    CALL(file.Open(log_file_name, Stream::APPEND));
    for(auto op: operations) {
        CALL(file.FeedOp(op));
        CALL(file.FeedString(",\n"));
    }
    CALL(file.DrainAll());
    CALL(file.Close());
    DONE;
}
