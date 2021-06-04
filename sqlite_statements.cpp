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


void BindNextParameter(
        const ColumnDescription& column,
        Tuple::AtomReader& reader,
        sqlite3_stmt* statement,
        const string& param_name
) {
    auto parameter_index = sqlite3_bind_parameter_index(statement, param_name.c_str());
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
        BindNextParameter(column, reader, statement, column.name);
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


void BindParametersForInsertStatement(
        const Op& op,
        const TableDescription& table,
        sqlite3_stmt* statement
) {
    auto op_parts = SplitOperation(op);
    auto op_key = op_parts.first;
    auto key_reader = op_key.ReadAtoms();

    for (const auto &column: table.pkey_columns) {
        BindNextParameter(column, key_reader, statement, column.name);
    }

    auto op_value = op_parts.second;
    auto value_reader = op_value.ReadAtoms();

    for(const auto &column: table.other_columns) {
        BindNextParameter(column, value_reader, statement, column.name);
    }
}


sqlite3_stmt* GenerateInsertPreparedStatement(
        const Op& op,
        sqlite3* db,
        const TableDescription& table
) {
    return GeneratePreparedStatement(op, db, table, GenerateInsertSqlStatement, BindParametersForInsertStatement);
}


string GenerateUpdateSqlStatement(const TableDescription& table) {
    /*
     * update <table_name> set
     *  <col_name> = @new_<col_name1>,
     *  <col_name> = @new_<col_name2>
     * where <col_name1> = @old_<col_name1> and <col_name2> = @old_<col_name2> and ...
     */

    auto set_expr = table.pkey_columns[0].name + " = @new_" + table.pkey_columns[0].name;
    auto where_expr = table.pkey_columns[0].name + " = @old_" + table.pkey_columns[0].name;
    for(auto i = 1; i < table.pkey_columns.size(); i++) {
        set_expr += ", " + table.pkey_columns[i].name + " = @new_" + table.pkey_columns[i].name;
        where_expr += " and " + table.pkey_columns[i].name + " = @old_" + table.pkey_columns[i].name;
    }

    for(const auto& column: table.other_columns) {
        set_expr += ", " + column.name + " = @new_" + column.name;
    }

    auto sql = "update " + table.name + " set " + set_expr + " where " + where_expr + ";";
    return sql;
}


void BindParametersForUpdateStatement(
        const Op& new_op,
        const Op& old_op,
        const TableDescription& table,
        sqlite3_stmt* statement
) {
    auto new_op_parts = SplitOperation(new_op);
    auto new_op_key = new_op_parts.first;
    auto new_key_reader = new_op_key.ReadAtoms();

    auto old_op_key = SplitOperation(old_op).first;
    auto old_key_reader = old_op_key.ReadAtoms();

    for (const auto &column: table.pkey_columns) {
        BindNextParameter(column, new_key_reader, statement, "new_" + column.name);
        BindNextParameter(column, old_key_reader, statement, "old_" + column.name);
    }

    auto new_op_value = new_op_parts.second;
    auto new_value_reader = new_op_value.ReadAtoms();

    for(const auto &column: table.other_columns) {
        BindNextParameter(column, new_value_reader, statement, "new_" + column.name);
    }
}


sqlite3_stmt* GenerateUpdatePreparedStatement(
        const Op& new_op,
        const Op& old_op,
        sqlite3* db,
        const TableDescription& table
) {
    sqlite3_stmt* statement = nullptr;

    auto sql = GenerateUpdateSqlStatement(table);
    const char* pz_tail = nullptr;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, &pz_tail);
    BindParametersForUpdateStatement(new_op, old_op, table, statement);

    return statement;
}