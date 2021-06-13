#include <iostream>
#include <map>
#include <vector>
#include "tcp_client.h"
#include "version_vector.h"
#include "log.h"
#include "sqlite3.h"

using namespace std;
using namespace ron;


const string SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 12345;


string GetDbName(uint64_t replica_id) {
    return "db_" + to_string(replica_id);
}


void Show(ReplicaState* replica_state) {
    sqlite3* db;
    sqlite3_open(GetDbName(replica_state->replica_id).c_str(), &db);

    auto sql = "select name, phone from phones";

    auto callback = [](void *param, int, char **argv, char **) -> int {
        cout << argv[0] << " " << argv[1] << endl;
        return 0;
    };

    sqlite3_exec(db, sql, callback, nullptr, nullptr);
    sqlite3_close(db);
}


void RunApp() {
    cout << "Sample App. Phone Book" << endl;

    while (true) {
        cout << "> ";
        string cmd;
        cin >> cmd;

        if (cmd == "exit") {
            break;
        } else if (cmd == "add") {

        } else if (cmd == "push") {

        } else if (cmd == "pull") {

        } else {
            cout << "Command is not recognized" << endl;
        }
    }
}


int main(int argc, char *argv[])
{
    uint64_t replica_id = strtoull(argv[1], nullptr, 10);
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

    sqlite3* db;
    sqlite3_open(GetDbName(replica_id).c_str(), &db);
    ReadVersionVector(replica_state->version_vector, db);
    replica_state->next_op_timestamp = GetNextOpTimestamp(replica_state);


    return 0;
}