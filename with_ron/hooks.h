#ifndef HOOKS_H
#define HOOKS_H

#include <vector>
#include "sqlite3.h"
#include "schema.h"

struct HookContext {
    uint64_t replica_id;
    int64_t logical_time;
    std::vector<TableDescription> tracked_tables;
};

void SetupHooks(sqlite3* db, HookContext* context);

#endif // !HOOKS_H