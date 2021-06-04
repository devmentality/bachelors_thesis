#ifndef MAINPROJ_VERSION_VECTOR_H
#define MAINPROJ_VERSION_VECTOR_H


std::pair<int64_t, int64_t> ReadVersionVector(uint64_t replica_id, sqlite3* db);
void SetupVersionVector(uint64_t replica_id, sqlite3* db);

#endif //MAINPROJ_VERSION_VECTOR_H
