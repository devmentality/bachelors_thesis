#include "schema.h"

#include <iostream>
#include <string>
#include "sqlite3.h"

using namespace std;


void TableDescription::BuildColumnsMapping() {
    for (const auto& col : pkey_columns) column_by_index[col.index] = col;
    for (const auto& col : other_columns) column_by_index[col.index] = col;
}


int TableDescription::ColumnsAmount() {
    return pkey_columns.size() + other_columns.size();
}


void Run(sqlite3* db, const string& sql) {
    char* zErrMsg = nullptr;

    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
}
