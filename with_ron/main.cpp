#include <iostream>
#include <string>
#include <vector>
#include "column_types.h"
#include "schema.h"
#include "hooks.h"
#include "sqlite3.h"


using namespace std;


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


void TestInsertWithHook(sqlite3* db) {
    string insert_record_sql =
        "insert into records(data, id) " \
        "values('Hellolleh21!', 21);";

    Run(db, insert_record_sql);
}

void TestDeleteWithHook(sqlite3* db) {
    string delete_record_sql = "delete from records where id = 1;";

    Run(db, delete_record_sql);
}

void TurnForeignKetSupportOn(sqlite3* db) {
    string sql = "PRAGMA foreign_keys = ON";

    Run(db, sql);
}


int main(int argn, char** args) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc = sqlite3_open("sample.db", &db);

    if (rc) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return 0;
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

    TestInsertWithHook(db);

    sqlite3_close(db);
    delete hook_context;
    return 0;
}
