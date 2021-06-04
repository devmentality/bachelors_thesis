#include <iostream>
#include <string>
#include "sqlite3.h"
#include "schema.h"

using namespace std;


void SetupCurrentOndxTable(sqlite3* db) {
    string create_table_sql =
            "create table curr_ondx (" \
            "value int not null default 0" \
        ")";

    string init_current_ondx =
            "insert into curr_ondx (value)" \
        "values (0)";

    Run(db, create_table_sql);
    Run(db, init_current_ondx);
}


void SetupVersionVector(uint64_t replica_id, sqlite3* db) {
    string create_table =
            "create table version_vector (" \
            "replica_id int not null primary key," \
            "ondx int not null," \
            "clock int not null" \
            ")";
    string insert_vector =
            "insert into version_vector (replica_id, ondx, clock) " \
            "values (" + to_string(replica_id) + ", 0, 0);";

    Run(db, create_table);
    Run(db, insert_vector);
}


pair<int64_t, int64_t> ReadVersionVector(uint64_t replica_id, sqlite3* db) {
    auto sql = "select ondx, clock from version_vector where replica_id = " + to_string(replica_id);
    auto result_ptr = new pair<int64_t, int64_t>;

    auto callback = [](void *result, int, char **argv, char **) -> int {
        auto ondx = atoll(argv[0]);
        auto clock = atoll(argv[1]);
        ((pair<int64_t, int64_t>*)result)->first = ondx;
        ((pair<int64_t, int64_t>*)result)->first = clock;
        return 0;
    };

    char* zErrMsg = nullptr;

    int rc = sqlite3_exec(db, sql.c_str(), callback, result_ptr, &zErrMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }

    auto result = *result_ptr;
    delete result_ptr;
    return result;
}



