#ifndef COLUMN_TYPES_H
#define COLUMN_TYPES_H

#include <set>
#include <map>
#include "sqlite3.h"

/*
	Sqlite support the following stored types (https://www.sqlite.org/datatype3.html):
		NULL, INTEGER, REAL, TEXT, BLOB

	But it has not strict type system and allows to store arbitrary types in one column.
	Column type in table description is "affinity" ~ preferred data type for the column.

	We force strict typing for compatibility with different databases.
*/

enum ColumnType {
	Integer,
	Float,
	Text
};


extern std::map<ColumnType, std::string> TYPE_AFFINITIES;

extern std::map<ColumnType, int> TO_SQLITE_TYPES;

extern std::set<int> SUPPORTED_DATA_TYPES;


#endif
