#pragma once
#include <string>
#include "sqlite3.h"

using namespace std;

void run(sqlite3* db, string sql);