#include "schema.h"

#include <iostream>
#include <vector>
#include <string>
#include "sqlite3.h"
#include "column_types.h"

using namespace std;


void TableDescription::BuildColumnsMapping() {
    for (auto col : pkey_columns) column_by_index[col.index] = col;
    for (auto col : other_columns) column_by_index[col.index] = col;
}

int TableDescription::ColumnsAmount() {
    return pkey_columns.size() + other_columns.size();
}


/*
    Makes sql of the form:

    create table __[table_name]_ops (
        pkey_col1 type1,
        pkey_col2 type2,
        ...,
        ondx int not null,
        operation text not null,
        primary key (pkey_col1, pkey_col2, ...)
    )
*/
string MakeCreateOperationsTableSql(const TableDescription& table_description) {
    auto table_name = "__" + table_description.name + "_ops";

    auto sql = "create table " + table_name + "(";
    string column_names = "";

    for (auto column : table_description.pkey_columns) {
        sql += column.name + " " + TYPE_AFFINITIES[column.type] + ", ";
        column_names += column.name + ",";
    }
    column_names.pop_back();

    sql += "ondx int not null, ";
    sql += "operation text not null, ";
    sql += "primary key(" + column_names + ")";
    sql += ");";

    return sql;
}


string MakeDeleteFromOpsSql(const TableDescription& table_description, 
                            string trig_tbl_name) {
    auto ops_table_name = "__" + table_description.name + "_ops";

    auto sql = "delete from " + ops_table_name + " where ";
    sql += table_description.pkey_columns[0].name + " = " + trig_tbl_name + "." + 
           table_description.pkey_columns[0].name;
    for (auto i = 1; i < table_description.pkey_columns.size(); i++) {
        auto col_name = table_description.pkey_columns[i].name;
        sql += " and ";
        sql += col_name + " = " + trig_tbl_name + "." + col_name;
    }
    return sql;
}


string MakeInsertToOpsSql(const TableDescription& table_description, 
                                string op_name, string trig_tbl_name) {
    auto ops_table_name = "__" + table_description.name + "_ops";

    auto sql = "insert into " + ops_table_name + "(" + 
                table_description.pkey_columns[0].name;
    
    for (auto i = 1; i < table_description.pkey_columns.size(); i++)
        sql += ", " + table_description.pkey_columns[i].name;
    sql += ", ondx, operation)\n";

    sql += "values (" + trig_tbl_name + "." + 
            table_description.pkey_columns[0].name;

    for (auto i = 1; i < table_description.pkey_columns.size(); i++)
        sql += ", " + trig_tbl_name + "." + 
                table_description.pkey_columns[i].name;
    sql += ", (select value from curr_ondx), '" + op_name + "')";

    return sql;
}


/*
    Makes sql for triggers after certain operation.
    Trigger basic structure:

    1. increment ondx value
    2. delete data about old operation on the target row
    3. insert new data about operation on the target row
*/
string MakeTriggerSql(const TableDescription& table_description, 
                            string op_name, string trig_tbl_name) {
    auto sql = "create trigger __" + table_description.name + "_" + op_name + "\n";
    sql += "after " + op_name + " on " + table_description.name + " for each row\n";
    sql += "begin\n";

    sql += "update curr_ondx set value = value + 1;\n";

    sql += MakeDeleteFromOpsSql(table_description, trig_tbl_name) + ";\n";
    sql += MakeInsertToOpsSql(table_description, op_name, trig_tbl_name) + ";\n";

    sql += "end;";

    return sql;
}


void Run(sqlite3* db, string sql) {
    char* zErrMsg = 0;

    int rc = sqlite3_exec(db, sql.c_str(), NULL, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
}


void CreateOperationsTable(sqlite3* db, const TableDescription& table_description) {
    auto sql = MakeCreateOperationsTableSql(table_description);
    Run(db, sql);
}


void CreateTriggers(sqlite3* db, const TableDescription& table_description) {
    pair<string, string> op_tbl[2]{ {"insert", "new"}, {"delete", "old"} };

    for (auto entry : op_tbl) {
        auto op_name = entry.first, trig_tbl_name = entry.second;
        auto sql = MakeTriggerSql(table_description, op_name, trig_tbl_name);
        Run(db, sql);
    }
}


void SetupCurrentOndxTable(sqlite3* db) {
    string create_table_sql = 
        "create table curr_ondx (" \
            "value int not null default 0" \
        ")";

    string init_current_ondx =
        "insert into curr_ondx (value)" \
        "values (0)";

    Run(db, create_table_sql);
    Run(db, init_current_ondx);
}
