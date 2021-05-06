#include <iostream>
#include "sqlite3.h"
using namespace std;

void MyHook(
    void* context,                      /* Copy of third arg to preupdate_hook() */
    sqlite3* db,                        /* Database handle */
    int operation,                      /* SQLITE_UPDATE, DELETE or INSERT */
    char const* database_name,          /* Database name */
    char const* table_name,             /* Table name */
    sqlite3_int64 old_rowid,            /* Rowid of row about to be deleted/updated */
    sqlite3_int64 new_rowid             /* New rowid value (for a rowid UPDATE) */
) {
    if (operation == SQLITE_INSERT) {
        int col_count = sqlite3_preupdate_count(db);
        for (int i = 0; i < col_count; i++) {
            sqlite3_value* val;
            sqlite3_preupdate_new(db, i, &val);
            cout << sqlite3_value_text(val)
                 << " (" << sqlite3_value_type(val) << ") ;";
        }
        cout << endl;
    }
}


int MyCommitCallback(void* ctx) {
    cout << "Commit invoked" << endl;
    return 0;
}


void SetupHooks(sqlite3* db) {
    sqlite3_preupdate_hook(db, MyHook, NULL);
    sqlite3_commit_hook(db, MyCommitCallback, NULL);
}