#include <iostream>
#include <map>
#include <vector>
#include "version_vector.h"
#include "log.h"
#include "hooks.h"
#include "sqlite3.h"
#include "tcp_client.h"

using namespace std;
using namespace ron;


const string SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 12345;


string GetDbName(uint64_t replica_id) {
    return "db_" + to_string(replica_id);
}

string GetLogName(uint64_t replica_id) {
    return "log_" + to_string(replica_id) + ".txt";
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


void Add(ReplicaState* replica_state, const string& name, const string& phone) {
    sqlite3* db;
    sqlite3_open(GetDbName(replica_state->replica_id).c_str(), &db);
    SetupHooks(db, replica_state);
    Begin(db, replica_state);

    string sql =
            "insert into phones(name, phone)"\
            "values(@name, @phone)";

    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare_v2(db, sql.c_str(), sql.length(), &stmt, nullptr);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@name"), name.c_str(), name.length(), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, sqlite3_bind_parameter_index(stmt, "@phone"), phone.c_str(), phone.length(), SQLITE_TRANSIENT);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    Commit(db, replica_state);
    sqlite3_close(db);
}


void Push(ReplicaState* replica_state) {
    int socket = Connect(SERVER_IP, SERVER_PORT);
    if (socket == -1) {
        cout << "Server is not available" << endl;
        return;
    }
    SendCmd(socket, "push");
    auto ondx = FetchOndx(socket, replica_state->replica_id);
    vector<Op> log_ops;
    MUST_OK(ReadLog(log_ops, GetLogName(replica_state->replica_id)), "read failed");
    vector<Op> patch;
    for(int64_t i = ondx; i < log_ops.size(); i++) {
        patch.push_back(log_ops[i]);
    }

    PushChanges(socket, replica_state->version_vector, patch);
}


void RunApp(ReplicaState* replica_state) {
    cout << "Sample App. Phone Book" << endl;
    Show(replica_state);
    cout << endl << endl;

    while (true) {
        cout << "> ";
        string cmd;
        cin >> cmd;

        if (cmd == "exit") {
            break;
        } else if (cmd == "add") {
            string name, phone;
            cin >> name >> phone;
            Add(replica_state, name, phone);
        } else if (cmd == "push") {
            Push(replica_state);
        } else if (cmd == "pull") {

        } else {
            cout << "Command is not recognized" << endl;
        }

        Show(replica_state);
        cout << endl << endl;
    }
}


void OnFirstLaunch(uint64_t replica_id) {
    sqlite3* db;
    sqlite3_open(GetDbName(replica_id).c_str(), &db);

    auto sql =
            "create table phones ( "\
            "  name text not null primary key, "\
            "  phone text not null)";

    Run(db, sql);
    SetupVersionVector(replica_id, db);

    sqlite3_close(db);
}


int main(int argc, char *argv[])
{
    uint64_t replica_id = strtoull(argv[1], nullptr, 10);

    if (argc > 2 && (strcmp(argv[2], "--first") == 0))
        OnFirstLaunch(replica_id);

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
    replica_state->log_file_name = GetLogName(replica_id);

    sqlite3* db;
    sqlite3_open(GetDbName(replica_id).c_str(), &db);
    ReadVersionVector(replica_state->version_vector, db);
    replica_state->next_op_timestamp = GetNextOpTimestamp(replica_state);

    sqlite3_close(db);

    RunApp(replica_state);

    return 0;
}