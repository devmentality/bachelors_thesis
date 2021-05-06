#include <iostream>
#include <string>
#include "schema.h"
#include "hooks.h"
#include "sqlite3.h"

using namespace std;


void CreateTable(sqlite3 *db, string sql, 
                 const TableDescription& table_description) {
    Run(db, sql);
    CreateOperationsTable(db, table_description);
    CreateTriggers(db, table_description);
}


void TestSchemaSetup(sqlite3 *db) {
    SetupCurrentOndxTable(db);

    string create_table_sql = 
        "create table records("  \
        "id int primary key not null, " \
        "data text not null);";

    TableDescription table_description("records",
        vector<ColumnDescription> {ColumnDescription("id", "int")}
    );

    CreateTable(db, create_table_sql, table_description);

    string insert_record_sql =
        "insert into records(id, data) " \
        "values(1, 'Hello!');";
    Run(db, insert_record_sql);

    string delete_record_sql = "delete from records where id = 1";
    Run(db, delete_record_sql);
}


void TestInsertWithHook(sqlite3* db) {
    string insert_record_sql =
        "insert into records(id, data) " \
        "values(2, 'Hello!');";

    Run(db, insert_record_sql);
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
    
    SetupHooks(db);
    //TestInsertWithHook(db);

    sqlite3_close(db);
    return 0;
}
