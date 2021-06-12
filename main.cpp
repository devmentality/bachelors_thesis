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
#include "sqlite_statements.h"


using namespace std;
using namespace ron;


//void TestSchemaSetup(sqlite3 *db, const TableDescription& sample_table) {
//    // SetupCurrentOndxTable(db);
//
//    string create_table_sql =
//        "create table records("  \
//        "id int primary key not null, " \
//        "data text not null);";
//
//    // CreateTable(db, create_table_sql, sample_table);
//
//    string insert_record_sql =
//        "insert into records(id, data) " \
//        "values(1, 'Hello!');";
//    Run(db, insert_record_sql);
//
//    string delete_record_sql = "delete from records where id = 1";
//    Run(db, delete_record_sql);
//}
//
//
//void TestInsertWithHook(sqlite3* db, string id, ReplicaState* context) {
//    Begin(db, context);
//    string insert_record_sql =
//        "insert into records(data, id) " \
//        "values('Hellolleh21!', " + id + ");";
//
//    Run(db, insert_record_sql);
//    Commit(db, context);
//}
//
//
//void TestDeleteWithHook(sqlite3* db) {
//    string delete_record_sql = "delete from records where id = 1;";
//
//    Run(db, delete_record_sql);
//}


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


//void RunDb() {
//    sqlite3* db;
//    char* zErrMsg = nullptr;
//    int rc = sqlite3_open("sample.db", &db);
//
//    if (rc) {
//        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
//        return;
//    }
//    cout << "Opened database successfully" << endl;
//
//    uint64_t replica_id = 12345;
//
//    vector<TableDescription> tracked_tables {
//            TableDescription(
//                    "records",
//                    vector<ColumnDescription> {ColumnDescription(0, "id", ColumnType::Integer)},
//                    vector<ColumnDescription> {ColumnDescription(1, "data", ColumnType::Text)}
//            )
//    };
//
//    //SetupVersionVector(replica_id, db);
//
//    string create_table_sql =
//            "create table records("  \
//            "id int primary key not null, " \
//            "data text not null);";
//
//    //Run(db, create_table_sql);
//
//    auto replica_state = new ReplicaState;
//    replica_state->replica_id = replica_id;
//    replica_state->tracked_tables = tracked_tables;
//    replica_state->is_merging = false;
//    ReadVersionVector(replica_state->version_vector, db);
//    replica_state->next_op_timestamp = GetNextOpTimestamp(replica_state);
//
//    SetupHooks(db, replica_state);
//
//    TestInsertWithHook(db, "33", replica_state);
//    TestInsertWithHook(db, "34", replica_state);
//    TestInsertWithHook(db, "35", replica_state);
//
//    sqlite3_close(db);
//    delete replica_state;
//}


//void RunLogMerge() {
//    vector<Op> log_ops;
//    vector<Op> patch;
//    MUST_OK(ReadLog(log_ops, "ron_log.txt"), "read failed");
//    MUST_OK(ReadLog(patch, "patch.txt"), "read failed");
//
//    vector<Op> new_ops;
//
//    MergeLogs(new_ops, log_ops, patch);
//
//    for(auto op: new_ops) {
//        cout << op.ID().String() << endl;
//    }
//
//    vector<DbOperation> db_ops;
//    GenerateResultingOperations(db_ops, log_ops, new_ops);
//
//    for(auto db_op: db_ops) {
//        cout << db_op.sql_operation << " " << db_op.new_operation.ID().String() << endl;
//    }
//}


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

    // new ron ops to add to local oplog
    vector<Op> new_ops;
    MergeLogs(new_ops, log_ops, patch);
    for(auto op: new_ops) {
        cout << op.ID().String() << endl;
    }

    vector<DbOperation> db_ops;
    GenerateResultingOperations(db_ops, log_ops, new_ops);

    SerializeToRon("ron_log.txt", new_ops);

    replica_state->is_merging = true;

    Begin(db, replica_state);
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
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    UpdateVersionVector(db, replica_state->version_vector, remote_clock);
    UpdateReplicaOndx(db, remote_replica_id,
                      replica_state->version_vector[remote_replica_id].ondx + patch.size());
    UpdateReplicaOndx(db, replica_state->replica_id,
                      replica_state->version_vector[replica_state->replica_id].ondx + new_ops.size());
    replica_state->next_op_timestamp = GetNextOpTimestamp(replica_state);
    Commit(db, replica_state);

    replica_state->is_merging = false;
}


int main(int argn, char** args) {
    TestFullMerge();
    return 0;
}
