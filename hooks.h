#ifndef HOOKS_H
#define HOOKS_H

#include <vector>
#include <map>
#include "sqlite3.h"
#include "schema.h"
#include "replica_state.h"
#include "ron/op.hpp"

void SetupHooks(sqlite3* db, ReplicaState* context);
void Begin(sqlite3* db);
void Commit(sqlite3* db, ReplicaState* context);

#endif // !HOOKS_H