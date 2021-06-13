#include "merge.h"

#include <vector>
#include <iostream>
#include <map>
#include "schema.h"
#include "version_vector.h"
#include "log.h"
#include "lww.h"
#include "ron/op.hpp"
#include "sqlite3.h"
#include "replica_state.h"
#include "sqlite_statements.h"

using namespace std;
using namespace ron;


void MergeReplicas(
        const TableDescription& table,
        sqlite3* db,
        ReplicaState* replica_state,
        const vector<Op>& log_ops,
        uint64_t remote_replica_id,
        const vector<Op>& patch,
        const map<uint64_t, Version>& remote_clock
) {
    // new ron ops to add to local oplog
    vector<Op> new_ops;
    MergeLogs(new_ops, log_ops, patch);
//    for(auto op: new_ops) {
//        cout << op.ID().String() << endl;
//    }

    vector<DbOperation> db_ops;
    GenerateResultingOperations(db_ops, log_ops, new_ops);

    replica_state->is_merging = true;

    Run(db, "begin transaction;");
    for(auto db_op: db_ops) {
        sqlite3_stmt* stmt = nullptr;
        if (db_op.sql_operation == "insert") {
            stmt = GenerateInsertPreparedStatement(db_op.new_operation, db, table);
        } else if (db_op.sql_operation == "delete") {
            stmt = GenerateDeletePreparedStatement(db_op.new_operation, db, table);
        } else if (db_op.sql_operation == "update") {
            stmt = GenerateUpdatePreparedStatement(db_op.new_operation, db_op.old_operation, db, table);
        }

        if (stmt != nullptr) {
            auto status = sqlite3_step(stmt);
            if (status != SQLITE_DONE) {
                cout << status << endl;
            }
            sqlite3_finalize(stmt);
        }
    }

    UpdateVersionVector(db, replica_state->version_vector, remote_clock);
    UpdateReplicaOndx(db, remote_replica_id,
                      replica_state->version_vector[remote_replica_id].ondx + patch.size());
    UpdateReplicaOndx(db, replica_state->replica_id,
                      replica_state->version_vector[replica_state->replica_id].ondx + new_ops.size());
    replica_state->next_op_timestamp = GetNextOpTimestamp(replica_state);
    SerializeToRon(replica_state->log_file_name, new_ops);
    Run(db, "commit transaction;");

    replica_state->is_merging = false;
}