#pragma once
#include <string>
#include <vector>
#include "sqlite3.h"

struct ColumnDescription {
    std::string name;
    std::string type;

    ColumnDescription(std::string name, std::string type) {
        this->name = name;
        this->type = type;
    }

    ColumnDescription() {}
};


struct TableDescription {
    std::string name;
    std::vector<ColumnDescription> pkey_columns;

    TableDescription(std::string name, 
                     const std::vector<ColumnDescription>& pkey_columns) {
        this->name = name;
        this->pkey_columns = pkey_columns;
    }

    TableDescription() {}
};


void Run(sqlite3* db, std::string sql);
void SetupCurrentOndxTable(sqlite3* db);
void CreateOperationsTable(sqlite3* db, const TableDescription& table_description);
void CreateTriggers(sqlite3* db, const TableDescription& table_description);