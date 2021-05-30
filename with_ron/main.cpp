#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"
#include "ron/ron-streams.hpp"
#include "column_types.h"
#include "schema.h"
#include "hooks.h"
#include "log.h"


using namespace std;
using namespace ron;


void CreateTable(sqlite3 *db, const string& sql,
                 const TableDescription& table_description) {
    Run(db, sql);
    CreateTriggers(db, table_description);
}


void TestSchemaSetup(sqlite3 *db, const TableDescription& sample_table) {
    SetupCurrentOndxTable(db);

    string create_table_sql = 
        "create table records("  \
        "id int primary key not null, " \
        "data text not null);";

    CreateTable(db, create_table_sql, sample_table);

    string insert_record_sql =
        "insert into records(id, data) " \
        "values(1, 'Hello!');";
    Run(db, insert_record_sql);

    string delete_record_sql = "delete from records where id = 1";
    Run(db, delete_record_sql);
}


void TestInsertWithHook(sqlite3* db, string id) {
    string insert_record_sql =
        "insert into records(data, id) " \
        "values('Hellolleh21!', " + id + ");";

    Run(db, insert_record_sql);
}

void TestDeleteWithHook(sqlite3* db) {
    string delete_record_sql = "delete from records where id = 1;";

    Run(db, delete_record_sql);
}

void TurnForeignKeySupportOn(sqlite3* db) {
    string sql = "PRAGMA foreign_keys = ON";

    Run(db, sql);
}


PROC WriteLog() {
    RONtStream file{};
    CALL(file.Open("ron_log.txt", Stream::WRITE));

    auto id = Uuid::Lamport(1, 1);
    auto ref = Uuid{"unknown"};
    CALL(file.FeedOp(Op{id, ref, "kek"}));
    ++id; ++id;
    CALL(file.FeedOp(Op{id, ref, "kek2"}));

    CALL(file.DrainAll());
    CALL(file.Close());
}





void RunDb() {
    sqlite3* db;
    char* zErrMsg = nullptr;
    int rc = sqlite3_open("sample.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return;
    }
    cout << "Opened database successfully" << endl;
    //SetupCurrentOndxTable(db);

    vector<TableDescription> tracked_tables {
            TableDescription(
                    "records",
                    vector<ColumnDescription> {ColumnDescription(0, "id", ColumnType::Integer)},
                    vector<ColumnDescription> {ColumnDescription(1, "data", ColumnType::Text)}
            )
    };

    string create_table_sql =
            "create table records("  \
            "id int primary key not null, " \
            "data text not null);";

    //CreateTable(db, create_table_sql, tracked_tables[0]);

    auto hook_context = new HookContext;
    hook_context->logical_time = ReadCurrentLogicalTimestamp(db);
    hook_context->replica_id = 1;
    hook_context->tracked_tables = tracked_tables;

    SetupHooks(db, hook_context);

    TestInsertWithHook(db, "17");
    TestInsertWithHook(db, "18");
    TestInsertWithHook(db, "19");

    sqlite3_close(db);
    delete hook_context;
}


int main(int argn, char** args) {
    //MUST_OK(WriteLog(), "write failed");
    vector<Op> ops;
    MUST_OK(ReadLog(ops, "ron_log.txt"), "read failed");
    //RunDb();
    for(auto op: ops)
        cout << op.ID().String() << endl;

    auto mapping = BuildPrimaryKeyMapping(ops);

    for(auto item: *mapping) {
        cout << item.first << " " << item.second.ID().String() << endl;
    }

    delete mapping;
    return 0;
}
