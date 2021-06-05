#ifndef MAINPROJ_REPLICA_STATE_H
#define MAINPROJ_REPLICA_STATE_H

#include <vector>
#include <map>

struct Version {
    int64_t ondx;
    int64_t clock;

    Version() {
        ondx = -1; clock = -1;
    }

    Version(int64_t ondx, int64_t clock) {
        this->ondx = ondx;
        this->clock = clock;
    }
};


struct ReplicaState {
    uint64_t replica_id;
    int64_t logical_time;
    std::vector<TableDescription> tracked_tables;
    std::vector<ron::Op> transaction_ops;
    std::map<uint64_t, Version> version_vector;
    bool is_merging;
};


#endif //MAINPROJ_REPLICA_STATE_H
