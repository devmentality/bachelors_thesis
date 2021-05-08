#pragma once
#include <vector>
#include "sqlite3.h"
#include "schema.h"

void SetupHooks(sqlite3* db, std::vector<TableDescription>* tracked_table_names);