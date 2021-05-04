#include <iostream>
#include <string>
#include "schema.h"
#include "sqlite3.h"

using namespace std;


void create_table(sqlite3 *db, string sql, const table_description& table_description)
{
    run(db, sql);
    create_operations_table(db, table_description);
    create_triggers(db, table_description);
}


int main(int argn, char** args) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc = sqlite3_open("sample.db", &db);

    if (rc)
    {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return 0;
    }
    cout << "Opened database successfully" << endl;
    

    //setup_current_ondx_table(db);

    //string create_table_sql = 
    //    "create table records("  \
    //    "id int primary key not null, " \
    //    "data text not null);";

    //table_description table_description("records",
    //    vector<column_description> {column_description("id", "int")}
    //);

    //create_table(db, create_table_sql, table_description);

    /*string insert_record_sql =
        "insert into records(id, data) " \
        "values(1, 'Hello!');";

    run(db, insert_record_sql);*/

   /* string delete_record_sql = "delete from records where id = 1";
    run(db, delete_record_sql);*/
   
    sqlite3_close(db);
    return 0;
}
