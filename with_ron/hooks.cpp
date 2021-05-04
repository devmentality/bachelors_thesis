#include <iostream>
#include "sqlite3.h"
using namespace std;

void my_hook(
    void* pCtx,                   /* Copy of third arg to preupdate_hook() */
    sqlite3* db,                  /* Database handle */
    int op,                       /* SQLITE_UPDATE, DELETE or INSERT */
    char const* zDb,              /* Database name */
    char const* zName,            /* Table name */
    sqlite3_int64 iKey1,          /* Rowid of row about to be deleted/updated */
    sqlite3_int64 iKey2           /* New rowid value (for a rowid UPDATE) */
)
{
    if (op == SQLITE_INSERT)
    {
        int col_count = sqlite3_preupdate_count(db);
        for (int i = 0; i < col_count; i++)
        {
            sqlite3_value* val;
            sqlite3_preupdate_new(db, i, &val);
            cout << sqlite3_value_text(val)
                << " (" << sqlite3_value_type(val) << ") ;";
        }
        cout << endl;
    }
}


int my_commit_callback(void* ctx)
{
    cout << "Commit invoked" << endl;
    return 0;
}


void setup_hooks(sqlite3* db)
{
    sqlite3_preupdate_hook(db, my_hook, NULL);
    sqlite3_commit_hook(db, my_commit_callback, NULL);
}