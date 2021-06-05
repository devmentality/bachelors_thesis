#ifndef MAINPROJ_VERSION_VECTOR_H
#define MAINPROJ_VERSION_VECTOR_H

#include <map>
#include "sqlite3.h"


void ReadVersionVector(std::map<uint64_t, Version>& version_vector, sqlite3* db);
void SetupVersionVector(uint64_t replica_id, sqlite3* db);
void UpdateVersionVector(
        sqlite3* db,
        const std::map<uint64_t, Version>& local,
        const std::map<uint64_t, Version>& remote
);
void MoveVector(sqlite3* db, uint64_t replica_id, int ondx_delta, int clock_delta);

#endif //MAINPROJ_VERSION_VECTOR_H
