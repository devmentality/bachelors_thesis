#include "tcp_server.h"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include "ron/op.hpp"
#include "ron/ron-streams.hpp"
#include "socket_io.h"
#include "merge.h"
#include "sqlite3.h"
#include "log.h"

using namespace std;
using namespace ron;


string ReadCommand(int client_socket) {
    char cmd[5] = {0};
    recv(client_socket, &cmd, sizeof cmd, 0);
    return string(cmd);
}


void HandlePush(int client_socket, ReplicaState* replica_state) {
    uint64_t remote_replica_id;
    recv(client_socket, &remote_replica_id, sizeof remote_replica_id, 0);
    auto ondx = replica_state->version_vector[remote_replica_id].ondx;
    send(client_socket, &ondx, sizeof ondx, 0);

    map<uint64_t, Version> remote_version_vector;
    vector<Op> patch;

    ReadVersionVector(remote_version_vector, client_socket);
    ReadRonPatch(patch, client_socket);

    vector<Op> server_log;
    MUST_OK(ReadLog(server_log, "server_log.txt"), "read failed");

    sqlite3* db;
    sqlite3_open("db_server", &db);

    MergeReplicas(replica_state->tracked_tables[0], db, replica_state, server_log, remote_replica_id, patch, remote_version_vector);

    sqlite3_close(db);
}


void HandlePull(int client_socket, ReplicaState* replica_state) {
    int64_t ondx;
    recv(client_socket, &ondx, sizeof ondx, 0);

    vector<Op> server_log;
    MUST_OK(ReadLog(server_log, "server_log.txt"), "read failed");

    vector<Op> patch;
    for(auto i = ondx; i < server_log.size(); i++) {
        patch.push_back(server_log[i]);
    }

    SendVersionVector(client_socket, replica_state->version_vector);
    SendPatch(client_socket, patch);
}


void ServeClient(int client_socket, ReplicaState* replica_state) {
    cout << "Awaiting client response" << endl;

    auto cmd = ReadCommand(client_socket);
    cout << "Client's command: " + cmd << endl;

    if(cmd == "push") {
        HandlePush(client_socket, replica_state);
    } else if (cmd == "pull") {
        HandlePull(client_socket, replica_state);
    }

    close(client_socket);
    cout << "Connection closed" << endl;
}


void Run(int port, ReplicaState* replica_state) {
    //setup a socket and connection tools
    sockaddr_in server_address;
    bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) {
        cerr << "Error establishing the server socket" << endl;
        return;
    }

    int bind_status = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if(bind_status < 0) {
        cerr << "Error binding socket to local address" << endl;
        return;
    }
    cout << "Waiting for a client to connect" << endl;
    listen(server_socket, 5);

    while (true) {
        sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);

        int client_socket = accept(server_socket, (sockaddr *)&client_address, &client_address_size);
        if(client_socket < 0) {
            cerr << "Error accepting request from client" << endl;
            exit(1);
        }
        cout << "Connected with client" << endl;

        ServeClient(client_socket, replica_state);
    }

    close(server_socket);
}

