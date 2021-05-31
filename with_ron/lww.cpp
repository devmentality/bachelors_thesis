#include <map>
#include <vector>
#include <string>
#include "ron/ron-streams.hpp"
#include "ron/const.hpp"
#include "sqlite3.h"
#include "schema.h"

using namespace ron;
using namespace std;


Tuple GetKeyTuple(const Op& op) {
    auto reader = op.Values().ReadAtoms();
    Tuple pkey;

    while(true) {
        Tuple::Box b;
        reader.ReadAtom(b);

        if (b == Uuid{"~"})
            break;

        pkey.PushBox(b);
    }

    return pkey;
}


map<size_t, Op>* BuildPrimaryKeyMapping(const vector<Op> &operations) {
    hash<Tuple> h{};
    auto mapping = new map<size_t, Op>();

    for(auto op: operations) {
        auto pkey = GetKeyTuple(op);
        auto key_hash = h(pkey);

        if (mapping->find(key_hash) != mapping->end()) {
            auto prev_op = (*mapping)[key_hash];

            if (prev_op.ID() < op.ID())
                (*mapping)[key_hash] = op;
        } else {
            (*mapping)[key_hash] = op;
        }
    }

    return mapping;
}


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


void BindParametersForDeleteStatement(const Op& op, const TableDescription& table, sqlite3_stmt* statement) {

}


sqlite3_stmt* GenerateDeletePreparedStatement(const Op& op, sqlite3* db, const TableDescription& table) {
    auto pkey = GetKeyTuple(op);
    auto reader = pkey.ReadAtoms();

    sqlite3_stmt* statement = nullptr;

    auto sql = GenerateDeleteSqlQuery(table);

    const char* pz_tail = nullptr;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &statement, &pz_tail);

    for(auto column: table.pkey_columns) {
        switch (column.type) {
            case ColumnType::Integer: {
                int64_t int_value;
                reader.ReadInteger(int_value);
                break;
            }
            case ColumnType::Text: {
                string text_value;
                reader.ReadUtf8String(text_value);
                break;
            }
            case ColumnType::Float: {
                double float_value;
                reader.ReadFloat(float_value);
                break;
            }
        }
    }

}

string GenerateInsertSqlStatement(const Op& op) {

}

