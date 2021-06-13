#include <iostream>
#include <vector>
#include <fstream>
#include "tcp_server.h"
#include "version_vector.h"


using namespace std;

const string SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 12345;


void OnFirstLaunch() {
    sqlite3* db;
    sqlite3_open("db_server", &db);

    auto sql =
            "create table phones ( "\
            "  name text not null primary key, "\
            "  phone text not null)";

    Run(db, sql);
    SetupVersionVector(0, db);

    ofstream log_file;
    log_file.open ("server_log.txt", ios::out);

    sqlite3_close(db);
}


int main(int argc, char *argv[])
{
    uint64_t replica_id = strtoull(argv[1], nullptr, 10);

    if (argc > 2 && (strcmp(argv[2], "--first") == 0))
        OnFirstLaunch();

    vector<TableDescription> tracked_tables {
            TableDescription(
                    "phones",
                    vector<ColumnDescription> {ColumnDescription(0, "name", ColumnType::Text)},
                    vector<ColumnDescription> {ColumnDescription(1, "phone", ColumnType::Text)}
            )
    };

    auto replica_state = new ReplicaState;
    replica_state->replica_id = replica_id;
    replica_state->tracked_tables = tracked_tables;
    replica_state->is_merging = false;
    replica_state->log_file_name = "server_log.txt";

    sqlite3* db;
    sqlite3_open("db_server", &db);
    ReadVersionVector(replica_state->version_vector, db);
    replica_state->next_op_timestamp = GetNextOpTimestamp(replica_state);

    sqlite3_close(db);

    Run(12345, replica_state);

    return 0;
}
