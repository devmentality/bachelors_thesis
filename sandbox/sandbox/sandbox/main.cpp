#include <iostream>
#include "sqlite3.h"
using namespace std;

/*
trigger

create trigger records_insert
after insert on records
for each row
begin
    update new
    set ondx = (select value from curr_ondx);

    update curr_ondx
    set value = value + 1;
end;

*/

static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    cout << "callback called with " << argc << " arguments" << endl;
    for (int i = 0; i < argc; i++)
    {
        cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << endl;
    return 0;
}


void create_table(sqlite3* db)
{
    char* zErrMsg = 0;
    const char* sql = "create table records("  \
        "id   int primary key     not null," \
        "data text                not null);";

    int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
}


void insert_data(sqlite3* db)
{
    char* zerrmsg = 0;
    const char* sql = "insert into records(id, data)" \
        "values(5, 'first');" \
        "insert into records(id, data)" \
        "values(6, 'second');";

    int rc = sqlite3_exec(db, sql, callback, 0, &zerrmsg);

    if (rc != SQLITE_OK)
    {
        cerr << "sql error: " << zerrmsg << endl;
        sqlite3_free(zerrmsg);
    }
    else
        cout << "records inserted successfully" << endl;
}

void select_data(sqlite3* db)
{
    char* zerrmsg = 0;
    const char* sql = "select * from records;";

    int rc = sqlite3_exec(db, sql, callback, 0, &zerrmsg);

    if (rc != SQLITE_OK)
    {
        cerr << "sql error: " << zerrmsg << endl;
        sqlite3_free(zerrmsg);
    }
    else
        cout << "records inserted successfully" << endl;
}

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
            sqlite3_value *val;
            sqlite3_preupdate_new(db, i, &val);
            cout << sqlite3_value_text(val) << " ";
        }
        cout << endl;
    }
}

int my_commit_callback(void* ctx)
{
    cout << "Commit invoked" << endl;
    return 0;
}

int main()
{
    sqlite3* db;
    char* zErrMsg = 0;
    int rc = sqlite3_open("sample.db", &db);

    if (rc)
    {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
        return 0;
    }
    cout << "Opened database successfully" << endl;
    sqlite3_preupdate_hook(db, my_hook, NULL);
    sqlite3_commit_hook(db, my_commit_callback, NULL);

    // create_table(db);
    insert_data(db);
    // select_data(db);

    

    sqlite3_close(db);
    return 0;
}