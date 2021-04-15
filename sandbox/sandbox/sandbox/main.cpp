#include <iostream>
#include <vector>
#include <string>
#include "sqlite3.h"
using namespace std;

struct column_description 
{
    string name;
    string type;
};

struct table_description 
{
    string name;
    vector<column_description> pkey_columns;
};

void create_operations_table(sqlite3* db, const table_description& table_description)
{ 
    auto sql = make_create_operations_table_sql(table_description);
    run(db, sql);
}

string make_create_operations_table_sql(const table_description& table_description)
{
    auto table_name = "__" + table_description.name + "_ops";

    auto sql = "create table " + table_name + "(";
    string column_names = "";

    for (auto column : table_description.pkey_columns)
    {
        sql += column.name + " " + column.type + ", ";
        column_names += column.name + ",";
    }
    column_names.pop_back();

    sql += "ondx int not null, ";
    sql += "operation text not null, ";
    sql += "primary key(" + column_names + ")";
    sql += ");";

    return sql;
}


void create_triggers(sqlite3* db, const table_description& table_description)
{
    pair<string, string> op_tbl[3]{ {"insert", "new"}, {"delete", "old"} };

    for (auto [op_name, trig_tbl_name] : op_tbl)
    {
        auto sql = make_trigger_sql(table_description, op_name, trig_tbl_name);
        run(db, sql);
    }
}

string make_trigger_sql(const table_description& table_description, string op_name, string trig_tbl_name)
{

    auto sql = "create trigger __" + table_description.name + "_" + op_name + "\n";
    sql += "after " + op_name + " on " + table_description.name + " for each row\n";
    sql += "begin\n";
    
    sql += "update curr_ondx set value = value + 1;\n";

    sql += make_delete_from_ops_sql(table_description, trig_tbl_name) + ";\n";
    sql += make_insert_to_ops_sql(table_description, op_name, trig_tbl_name) + ";\n";

    sql += "end;";

    return sql;
}

string make_delete_from_ops_sql(const table_description& table_description, string trig_tbl_name)
{
    auto ops_table_name = "__" + table_description.name + "_ops";

    auto sql = "delete from " + ops_table_name + " where ";
    sql += table_description.pkey_columns[0].name + " = " + trig_tbl_name + "." + table_description.pkey_columns[0].name;
    for (auto i = 1; i < table_description.pkey_columns.size(); i++)
    {
        auto col_name = table_description.pkey_columns[i].name;
        sql += " and ";
        sql += col_name + " = " + trig_tbl_name + "." + col_name;
    }
}

string make_insert_to_ops_sql(const table_description& table_description, string op_name, string trig_tbl_name)
{
    auto ops_table_name = "__" + table_description.name + "_ops";

    auto sql = "insert into " + ops_table_name + "(" + table_description.pkey_columns[0].name;
    for (auto i = 1; i < table_description.pkey_columns.size(); i++)
        sql += ", " + table_description.pkey_columns[i].name;

    sql += ", ondx, operation)\n";
    sql += "values (" + table_description.pkey_columns[0].name;
    for (auto i = 1; i < table_description.pkey_columns.size(); i++)
        sql += ", " + trig_tbl_name + "." + table_description.pkey_columns[i].name;

    sql += ", (select value from curr_ondx), '" + op_name + "')";
    
    return sql;
}


void run(sqlite3* db, string sql)
{
    char* zErrMsg = 0;

    int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
}


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
    const char* sql = "insert into dt_test(key, value)" \
        "values(5, 'first');" \
        "insert into dt_test(key, value)" \
        "values(6, '123');";

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
    const char* sql = "select * from dt_test;";

    int rc = sqlite3_exec(db, sql, callback, 0, &zerrmsg);

    if (rc != SQLITE_OK)
    {
        cerr << "sql error: " << zerrmsg << endl;
        sqlite3_free(zerrmsg);
    }
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
    // sqlite3_commit_hook(db, my_commit_callback, NULL);

    // create_table(db);
    insert_data(db);
    // select_data(db);

    

    sqlite3_close(db);
    return 0;
}