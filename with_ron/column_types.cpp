#include "column_types.h"

#include <set>
#include <map>
#include "sqlite3.h"

using namespace std;

map<ColumnType, string> TYPE_AFFINITIES{
	{ ColumnType::Integer, "int" },
	{ ColumnType::Float, "real" },
	{ ColumnType::Text, "text" }
};


map<ColumnType, int> TO_SQLITE_TYPES{
	{ ColumnType::Integer, SQLITE_INTEGER },
	{ ColumnType::Float, SQLITE_FLOAT },
	{ ColumnType::Text, SQLITE_TEXT }
};


set<int> SUPPORTED_DATA_TYPES{
	SQLITE_INTEGER,
	SQLITE_FLOAT,
	SQLITE_TEXT,
	//  SQLITE_BLOB       // for future       
};