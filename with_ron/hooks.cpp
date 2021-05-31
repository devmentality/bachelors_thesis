#include "hooks.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include "sqlite3.h"
#include "schema.h"
#include "log.h"
#include "column_types.h"
#include "ron/ron.hpp"


using namespace ron;
using namespace std;


bool TryGetTableDescription(
        const vector<TableDescription>& tables, 
        string name, 
        TableDescription &out
) {
    for(auto table: tables)
        if (table.name == name) {
            out = table;
            return true;
        }
    return false;
}


void PrintValues(const vector<sqlite3_value*>& values) {
    for (auto val : values)
    {
        cout << sqlite3_value_text(val)
            << " (" << sqlite3_value_type(val) << ") ;";
    }
    cout << endl;
}


bool AreValidTypes(
        const TableDescription& table, 
        const vector<sqlite3_value*>& values
) {
    for (auto index = 0; index < values.size(); index++) {
        auto col = table.column_by_index.at(index);
        auto expected_type = TO_SQLITE_TYPES[col.type];
        auto actual_type = sqlite3_value_type(values[index]); //  TODO: what if values is NULL?
        if (expected_type != actual_type)
            return false;
    }
    return true;
}


vector<sqlite3_value*> ReadValues(sqlite3* db, int(*fetch)(sqlite3*, int, sqlite3_value**)) {
    int col_count = sqlite3_preupdate_count(db);
    vector<sqlite3_value*> values(col_count);

    for (int i = 0; i < col_count; i++) {
        sqlite3_value* val;
        fetch(db, i, &val);
        values[i] = val;
    }

    return values;
}


vector<sqlite3_value*> ReadNewValues(sqlite3* db) {
    return ReadValues(db, sqlite3_preupdate_new);
}


vector<sqlite3_value*> ReadOldValues(sqlite3* db) {
    return ReadValues(db, sqlite3_preupdate_old);
}


void PushValuesForColumns(
        const vector<ColumnDescription>& columns, 
        const vector<sqlite3_value*>& values, 
        Op& op
) {
    for (auto column : columns) {
        auto value = values[column.index];
        auto type = sqlite3_value_type(value);

        switch (type)
        {
            case SQLITE_INTEGER:
                op.Values().PushInteger(sqlite3_value_int64(value));
                break;
            case SQLITE_FLOAT:
                op.Values().PushFloat(sqlite3_value_double(value));
                break;
            case SQLITE_TEXT:
                // sqlite3_value_text returns UTF-8 value
                op.Values().PushUtf8String(string(reinterpret_cast<const char*>(sqlite3_value_text(value))));
                break;
            default:
                break;
        }
    }
}


void UpdateLogicalTimestamp(sqlite3* db, HookContext* context) {
    auto values = ReadNewValues(db);
    auto new_logical_timestamp = sqlite3_value_int64(values[0]);
    context->logical_time = new_logical_timestamp;
}


Op MakeInsertOpFromValues(
        const TableDescription& table, 
        const vector<sqlite3_value*>& values,
        Uuid op_uuid
) {
    Op op{ op_uuid, Uuid{"unknown"} };  // Now ids are manual. Hpw to add refs?

    PushValuesForColumns(table.pkey_columns, values, op);
    op.Values().PushUuid(Uuid{ "~" });
    PushValuesForColumns(table.other_columns, values, op);
    
    return op;
}


Op MakeDeleteOpFromValues(
        const TableDescription& table,
        const vector<sqlite3_value*>& values,
        Uuid op_uuid
) {
    Op op{ op_uuid, Uuid{"unknown"} };  // Just fake id & ref. Will be generated by oplog?

    PushValuesForColumns(table.pkey_columns, values, op);
    op.Values().PushUuid(Uuid{ "~" });

    return op;
}


void HandleInsert(sqlite3 *db, const TableDescription& table, HookContext* context, Uuid op_uuid) {
    auto values = ReadNewValues(db);
    auto areValidTypes = AreValidTypes(table, values);
    cout << "Types are " << (areValidTypes ? "" : "not") << " valid" << endl;
    context->transaction_ops.push_back(MakeInsertOpFromValues(table, values, op_uuid));
}


void HandleDelete(sqlite3* db, const TableDescription& table, HookContext* context, Uuid op_uuid) {
    auto values = ReadOldValues(db);
    context->transaction_ops.push_back(MakeDeleteOpFromValues(table, values, op_uuid));
}



void PreupdateHook(
    void* ctx,                      /* Copy of third arg to preupdate_hook() */
    sqlite3* db,                        /* Database handle */
    int operation,                      /* SQLITE_UPDATE, DELETE or INSERT */
    char const* database_name,          /* Database name */
    char const* tbl_name,               /* Table name */
    sqlite3_int64 old_rowid,            /* Rowid of row about to be deleted/updated */
    sqlite3_int64 new_rowid             /* New rowid value (for a rowid UPDATE) */
) {
    auto context = (HookContext*)ctx;
    auto tracked_tables = context->tracked_tables;
    string table_name(tbl_name);

    if (table_name == "curr_ondx") {
        UpdateLogicalTimestamp(db, context);
        return;
    }

    TableDescription tbl;
    if (!TryGetTableDescription(tracked_tables, table_name, tbl))
        return;

    auto op_uuid = Uuid::Lamport(context->logical_time + 1, context->replica_id);
    if (operation == SQLITE_INSERT) {
        HandleInsert(db, tbl, context, op_uuid);
    } else if (operation == SQLITE_DELETE) {
        HandleDelete(db, tbl, context, op_uuid);
    } else {
        throw domain_error("operations other than INSERT & DELETE are prohibited");
    }
}


int CommitHook(void* ctx) {
    auto context = (HookContext*)ctx;
    cout << "Commit invoked" << endl;
    MUST_OK(SerializeToRon("ron_log.txt", context->transaction_ops), "transaction was not written");
    context->transaction_ops.clear();
    return 0;
}


void SetupHooks(sqlite3* db, HookContext* context) {
    sqlite3_preupdate_hook(db, PreupdateHook, context);
    sqlite3_commit_hook(db, CommitHook, context);
}