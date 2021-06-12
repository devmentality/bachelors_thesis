#include <iostream>
#include <string>
#include "sqlite3.h"
#include "schema.h"
#include "replica_state.h"

using namespace std;


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


void ReadVersionVector(map<uint64_t, Version>& version_vector, sqlite3* db) {
    auto sql = "select replica_id, ondx, clock from version_vector";

    auto callback = [](void *param, int, char **argv, char **) -> int {
        auto version_vector = (map<uint64_t, Version>*)param;
        auto replica_id = atoll(argv[0]);
        auto ondx = atoll(argv[1]);
        auto clock = atoll(argv[2]);
        (*version_vector)[replica_id] = Version(ondx, clock);
        return 0;
    };

    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql, callback, &version_vector, &zErrMsg);

    if (rc != SQLITE_OK) {
        cerr << "SQL error: " << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }
}


void InsertReplicaClock(sqlite3* db, uint64_t replica_id, int64_t clock) {
    auto sql = "insert into version_vector (replica_id, ondx, clock) " \
               "values (" + to_string(replica_id) + ", " + "0, " + to_string(clock) + ");";

    Run(db, sql);
}


void UpdateReplicaClock(sqlite3* db, uint64_t replica_id, int64_t clock) {
    auto sql = "update version_vector set " \
                "clock = " + to_string(clock) + " "
                "where replica_id = " + to_string(replica_id);

    Run(db, sql);
}


void UpdateVersionVector(
        sqlite3* db,
        map<uint64_t, Version>& local,
        const map<uint64_t, Version>& remote
) {
    for(auto item: remote) {
        auto replica_id = item.first;
        auto remote_clock = item.second.clock;

        if (local.find(replica_id) != local.end() &&
            local.at(replica_id).clock < remote_clock) {
            local[replica_id].clock = remote_clock;
            UpdateReplicaClock(db, replica_id, remote_clock);
        } else {
            local[replica_id].clock = remote_clock;
            InsertReplicaClock(db, replica_id, remote_clock);
        }
    }
}

// replica_id, ondx, clock
//void MoveVector(sqlite3* db, uint64_t replica_id, int ondx_delta, int clock_delta) {
//    auto sql = "update version_vector set " \
//                "ondx = ondx + " + to_string(ondx_delta) + ", " \
//                "clock = clock + " + to_string(clock_delta) + " " \
//                "where replica_id = " + to_string(replica_id);
//
//    Run(db, sql);
//}


void UpdateReplicaOndx(sqlite3* db, uint64_t replica_id, int64_t ondx) {
    auto sql = "update version_vector set " \
                "ondx = " + to_string(ondx) + " "
                "where replica_id = " + to_string(replica_id);

    Run(db, sql);
}


int64_t GetNextOpTimestamp(ReplicaState* replica_state) {
    int64_t max_clock = -1;
    for(auto item: replica_state->version_vector)
        max_clock = std::max(max_clock, item.second.clock);

    return max_clock + 1;
}


