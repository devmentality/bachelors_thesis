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
        "values('Hello!', 18);";

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
    TurnForeignKetSupportOn(db);

    auto tracked_tables = new vector<TableDescription> { 
        TableDescription(
            "records",
            vector<ColumnDescription> {ColumnDescription(0, "id", ColumnType::Integer)},
            vector<ColumnDescription> {ColumnDescription(1, "data", ColumnType::Text)}
        ),
        TableDescription(
            "master",
            vector<ColumnDescription> {ColumnDescription(0, "id", ColumnType::Integer)},
            vector<ColumnDescription> {ColumnDescription(1, "fkey", ColumnType::Integer)}
        )
    };

//    string sql =
//            "create table master ("
//            "id int primary key,"
//            "fkey int,"
//            "foreign key(fkey) references records(id) on delete set null"
//            ");";
//
//    CreateTable(db, sql, (*tracked_tables)[1]);


    SetupHooks(db, tracked_tables);
    TestDeleteWithHook(db);


    sqlite3_close(db);
    delete tracked_tables;
    return 0;
}
