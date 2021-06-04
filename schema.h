#ifndef SCHEMA_H
#define SCHEMA_H

#include <string>
#include <vector>
#include <map>
#include "sqlite3.h"
#include "column_types.h"
#include "ron/op.hpp"


struct ColumnDescription {
    int index;
    std::string name;
    ColumnType type;

    ColumnDescription(int index, std::string name, ColumnType type) {
        this->index = index;
        this->name = name;
        this->type = type;
    }

    ColumnDescription() {}
};


class TableDescription {
public:
    std::string name;
    std::vector<ColumnDescription> pkey_columns;
    std::vector<ColumnDescription> other_columns;
    std::map<int, ColumnDescription> column_by_index;

    TableDescription(
            std::string name,
            const std::vector<ColumnDescription>& pkey_columns,
            const std::vector<ColumnDescription>& other_columns
    ) {
        this->name = name;
        this->pkey_columns = pkey_columns;
        this->other_columns = other_columns;

        BuildColumnsMapping();
    }

    TableDescription() {}

    int ColumnsAmount();

private:
    void BuildColumnsMapping();
};


struct DbOperation {
    std::string sql_operation;
    ron::Op new_operation;
    ron::Op old_operation;

    DbOperation(std::string sql_operation, ron::Op new_operation){
        this->sql_operation = std::move(sql_operation);
        this->new_operation = std::move(new_operation);
    }
};


void Run(sqlite3* db, const std::string& sql);
void CreateTriggers(sqlite3* db, const TableDescription& table_description);

#endif // !SCHEMA_H


