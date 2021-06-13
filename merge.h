#ifndef MAINPROJ_MERGE_H
#define MAINPROJ_MERGE_H

#include <vector>
#include <map>
#include "schema.h"
#include "replica_state.h"

void MergeReplicas(
        const TableDescription& table,
        sqlite3* db,
        ReplicaState* replica_state,
        const std::vector<ron::Op>& log_ops,
        uint64_t remote_replica_id,
        const std::vector<ron::Op>& patch,
        const std::map<uint64_t, Version>& remote_clock
);

#endif //MAINPROJ_MERGE_H
