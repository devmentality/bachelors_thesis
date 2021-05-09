#include <iostream>
#include <vector>
#include <algorithm>
#include "sqlite3.h"
#include "schema.h"
#include "column_types.h"
#include "ron/ron.hpp"

using namespace ron;
using namespace std;


bool TryGetTableDescription(
        const vector<TableDescription>& tables, 
        string name, 
        TableDescription &out
) {
    for(auto table: tables)
        if (table.name == name) {
            out = table;
            return true;
        }
    return false;
}


void PrintValues(const vector<sqlite3_value*>& values) {
    for (auto val : values)
    {
        cout << sqlite3_value_text(val)
            << " (" << sqlite3_value_type(val) << ") ;";
    }
    cout << endl;
}

bool AreValidTypes(
        const TableDescription& table, 
        const vector<sqlite3_value*>& values
) {
    for (auto index = 0; index < values.size(); index++) {
        auto col = table.column_by_index.at(index);
        auto expected_type = TO_SQLITE_TYPES[col.type];
        auto actual_type = sqlite3_value_type(values[index]); //  TODO: what if values is NULL?
        if (expected_type != actual_type)
            return false;
    }
    return true;
}

vector<sqlite3_value*> ReadValues(sqlite3* db) {
    int col_count = sqlite3_preupdate_count(db);
    vector<sqlite3_value*> values(col_count);

    for (int i = 0; i < col_count; i++) {
        sqlite3_value* val;
        sqlite3_preupdate_new(db, i, &val);
        values[i] = val;
    }

    return values;
}


void HandleInsert(sqlite3 *db, const TableDescription& table) {
    auto values = ReadValues(db);
    PrintValues(values);

    auto areValidTypes = AreValidTypes(table, values);
    cout << "Types are " << (areValidTypes ? "" : "not") << " valid" << endl;
}


void MyHook(
    void* context,                      /* Copy of third arg to preupdate_hook() */
    sqlite3* db,                        /* Database handle */
    int operation,                      /* SQLITE_UPDATE, DELETE or INSERT */
    char const* database_name,          /* Database name */
    char const* tbl_name,               /* Table name */
    sqlite3_int64 old_rowid,            /* Rowid of row about to be deleted/updated */
    sqlite3_int64 new_rowid             /* New rowid value (for a rowid UPDATE) */
) {
    auto tracked_tables = (vector<TableDescription>*)context;
    string table_name(tbl_name);

    TableDescription tbl;
    if (!TryGetTableDescription(*tracked_tables, table_name, tbl))
        return;

    if (operation == SQLITE_INSERT) {
        HandleInsert(db, tbl);
    } else if (operation == SQLITE_DELETE) {
        ;
    } else {
        throw domain_error("operations other than INSERT & DELETE are prohibited");
    }
}


int MyCommitCallback(void* ctx) {
    cout << "Commit invoked" << endl;
    return 0;
}


void SetupHooks(sqlite3* db, vector<TableDescription> *tracked_tables) {
    sqlite3_preupdate_hook(db, MyHook, tracked_tables);
    sqlite3_commit_hook(db, MyCommitCallback, NULL);
}