#include "sqlite_statements.h"

#include "ron/ron-streams.hpp"
#include "sqlite3.h"
#include "schema.h"
#include "lww.h"


using namespace std;
using namespace ron;


string GenerateDeleteSqlQuery(const TableDescription& table) {
    auto sql = "delete from " + table.name + " where ";
    auto condition = table.pkey_columns[0].name + " = @" + table.pkey_columns[0].name;
    sql += condition;
    for (auto i = 1; i < table.pkey_columns.size(); i++) {
        condition = table.pkey_columns[i].name + " = @" + table.pkey_columns[i].name;
        sql += " and " + condition;
    }
    sql += ";";
    return sql;
}


void BindNextParameter(const ColumnDescription& column, Tuple::AtomReader& reader, sqlite3_stmt* statement) {
    auto parameter_index = sqlite3_bind_parameter_index(statement, column.name.c_str());
    switch (column.type) {
        case ColumnType::Integer: {
            int64_t int_value;
            reader.ReadInteger(int_value);
            sqlite3_bind_int64(statement, parameter_index, int_value);
            break;
        }
        case ColumnType::Text: {
            string text_value;
            reader.ReadUtf8String(text_value);
            sqlite3_bind_text(statement, parameter_index, text_value.c_str(), text_value.length(), SQLITE_TRANSIENT);
            break;
        }
        case ColumnType::Float: {
            double float_value;
            reader.ReadFloat(float_value);
            sqlite3_bind_double(statement, parameter_index, float_value);
            break;
        }
    }
}

// TODO: handle nulls
void BindParametersForDeleteStatement(const Op& op, const TableDescription& table, sqlite3_stmt* statement) {
    auto op_key = GetKeyTuple(op);
    auto reader = op_key.ReadAtoms();

    for(const auto& column: table.pkey_columns) {
        BindNextParameter(column, reader, statement);
    }
}


sqlite3_stmt* GeneratePreparedStatement(
        const Op& op, sqlite3* db, const TableDescription& table,
        string (* sql_generator)(const TableDescription&),
        void (* param_binder)(const Op&, const TableDescription&, sqlite3_stmt*)) {
    sqlite3_stmt* statement = nullptr;

    auto sql = sql_generator(table);
    const char* pz_tail = nullptr;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, &pz_tail);
    param_binder(op, table, statement);

    return statement;
}


sqlite3_stmt* GenerateDeletePreparedStatement(const Op& op, sqlite3* db, const TableDescription& table) {
    return GeneratePreparedStatement(op, db, table, GenerateDeleteSqlQuery, BindParametersForDeleteStatement);
}


string GenerateInsertSqlStatement(const TableDescription& table) {
    /*
     * insert into <table_name> (<col_name>, <col_name>, ...)
     * values (@<col_name>, @<col_name>, ...)
     */

    auto header = table.pkey_columns[0].name;
    auto values = "@" + table.pkey_columns[0].name;
    for(auto i = 1; i < table.pkey_columns.size(); i++) {
        header += ", " + table.pkey_columns[i].name;
        values += ", @" + table.pkey_columns[i].name;
    }

    for(const auto& column: table.other_columns) {
        header += ", " + column.name;
        values += ", @" + column.name;
    }

    auto sql = "insert into " + table.name + " (" + header + ") values (" + values + ");";
    return sql;
}


void BindParametersForInsertStatement(const Op& op, const TableDescription& table, sqlite3_stmt* statement) {
    auto op_key = GetKeyTuple(op);
    auto reader = op_key.ReadAtoms();

    for (const auto &column: table.pkey_columns) {
        BindNextParameter(column, reader, statement);
    }

    for(const auto &column: table.other_columns) {
        BindNextParameter(column, reader, statement);
    }
}


sqlite3_stmt* GenerateInsertPreparedStatement(const Op& op, sqlite3* db, const TableDescription& table) {
    return GeneratePreparedStatement(op, db, table, GenerateInsertSqlStatement, BindParametersForInsertStatement);
}