#pragma once
#include <string>
#include <vector>
#include "sqlite3.h"

struct column_description
{
    std::string name;
    std::string type;

    column_description(std::string _name, std::string _type)
    {
        name = _name;
        type = _type;
    }
};

struct table_description
{
    std::string name;
    std::vector<column_description> pkey_columns;

    table_description(std::string _name, const std::vector<column_description>& _pkey_columns)
    {
        name = _name;
        pkey_columns = _pkey_columns;
    }
};

void run(sqlite3* db, std::string sql);
void setup_current_ondx_table(sqlite3* db);
void create_operations_table(sqlite3* db, const table_description& table_description);
void create_triggers(sqlite3* db, const table_description& table_description);