#ifndef MAINPROJ_SQLITE_STATEMENTS_H
#define MAINPROJ_SQLITE_STATEMENTS_H

#include "ron/op.hpp"
#include "sqlite3.h"
#include "schema.h"

sqlite3_stmt* GenerateDeletePreparedStatement(const ron::Op& op, sqlite3* db, const TableDescription& table);
sqlite3_stmt* GenerateInsertPreparedStatement(const ron::Op& op, sqlite3* db, const TableDescription& table);


#endif //MAINPROJ_SQLITE_STATEMENTS_H
