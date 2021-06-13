#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"
#include "ron/ron-streams.hpp"
#include "column_types.h"
#include "schema.h"
#include "hooks.h"
#include "log.h"
#include "lww.h"
#include "version_vector.h"
#include "merge.h"


using namespace std;
using namespace ron;


pair<sqlite3*, ReplicaState*> InitDb() {
    sqlite3* db;
    char* zErrMsg = nullptr;
    int rc = sqlite3_open("sample.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        throw domain_error("cant open db");
    }
    cout << "Opened database successfully" << endl;

    uint64_t replica_id = 12345;

    vector<TableDescription> tracked_tables {
            TableDescription(
                    "records",
                    vector<ColumnDescription> {ColumnDescription(0, "id", ColumnType::Integer)},
                    vector<ColumnDescription> {ColumnDescription(1, "data", ColumnType::Text)}
            )
    };

    auto replica_state = new ReplicaState;
    replica_state->replica_id = replica_id;
    replica_state->tracked_tables = tracked_tables;
    replica_state->is_merging = false;
    ReadVersionVector(replica_state->version_vector, db);
    replica_state->next_op_timestamp = GetNextOpTimestamp(replica_state);

    SetupHooks(db, replica_state);

    return {db, replica_state};
}


void TestFullMerge() {
    auto config = InitDb();
    auto db = config.first;
    auto replica_state = config.second;

    TableDescription table(
            "records",
            vector<ColumnDescription> {ColumnDescription(0, "id", ColumnType::Integer)},
            vector<ColumnDescription> {ColumnDescription(1, "data", ColumnType::Text)}
    );

    vector<Op> log_ops;
    MUST_OK(ReadLog(log_ops, "ron_log.txt"), "read failed");

    // what we get from remote replica
    vector<Op> patch;
    MUST_OK(ReadLog(patch, "patch.txt"), "read failed");
    map<uint64_t, Version> remote_clock = {
            {1, Version(3, 6)},
    };
    uint64_t remote_replica_id = 1;

    MergeReplicas(table, db, replica_state, log_ops, remote_replica_id, patch, remote_clock);
}


int main(int argn, char** args) {
    TestFullMerge();
    return 0;
}
