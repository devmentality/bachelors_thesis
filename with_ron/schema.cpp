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


/*
    Makes sql for triggers after certain operation.
    Trigger's only responsibility is to increment curr_ondx value
*/
string MakeTriggerSql(const TableDescription& table_description, const string& op_name) {
    auto sql = "create trigger __" + table_description.name + "_" + op_name + "\n";
    sql += "after " + op_name + " on " + table_description.name + " for each row\n";
    sql += "begin\n";
    sql += "update curr_ondx set value = value + 1;\n";
    sql += "end;";

    return sql;
}


void Run(sqlite3* db, const string& sql) {
    char* zErrMsg = nullptr;

    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
}


void CreateTriggers(sqlite3* db, const TableDescription& table_description) {
    string operations[2]{ "insert" , "delete"  };

    for (const auto& operation : operations) {
        auto sql = MakeTriggerSql(table_description, operation);
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
