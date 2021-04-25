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
    return sql;
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


void create_operations_table(sqlite3* db, const table_description& table_description)
{
    auto sql = make_create_operations_table_sql(table_description);
    run(db, sql);
}


void create_triggers(sqlite3* db, const table_description& table_description)
{
    pair<string, string> op_tbl[3]{ {"insert", "new"}, {"delete", "old"} };

    for (auto entry : op_tbl)
    {
        auto op_name = entry.first, trig_tbl_name = entry.second;
        auto sql = make_trigger_sql(table_description, op_name, trig_tbl_name);
        run(db, sql);
    }
}



