#include <iostream>
#include <string>
#include "schema.h"
#include "sqlite3.h"

using namespace std;


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
    
    string create_table_sql = 
        "create table records("  \
        "id int primary key not null, " \
        "data text not null);";

    run(db, create_table_sql);
   
    sqlite3_close(db);
    return 0;
}
