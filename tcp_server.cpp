#include "tcp_server.h"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include "version_vector.h"
#include "ron/op.hpp"
#include "ron/ron-streams.hpp"

using namespace std;
using namespace ron;


string ReadCommand(int client_socket) {
    char cmd[5] = {0};
    recv(client_socket, (char*)&cmd, sizeof(cmd), 0);
    return string(cmd);
}


void ReadVersionVector(map<uint64_t, Version>& remote_version_vector, int client_socket) {
    int items_count;

    recv(client_socket, &items_count, sizeof items_count, 0);
    cout << "VV rows count: " << items_count << endl;

    for(auto i = 0; i < items_count; i++) {
        uint64_t replica_id;
        int64_t clock;

        recv(client_socket, &replica_id, sizeof replica_id, 0);
        recv(client_socket, &clock, sizeof clock, 0);

        cout << replica_id << " -> " << clock << endl;

        remote_version_vector[replica_id].ondx = 0;
        remote_version_vector[replica_id].clock = clock;
    }
}


void ReadRonPatch(vector<Op>& patch, int client_socket) {
    uint64_t patch_size;
    recv(client_socket, &patch_size, sizeof patch_size, 0);
    uint64_t read_bytes = 0;
    char *buffer = new char[patch_size];

    while (read_bytes < patch_size) {
        auto received_bytes = recv(client_socket, buffer + read_bytes, patch_size - read_bytes, 0);
        read_bytes += received_bytes;
    }

    string patch_string = buffer;
    cout << "PATCH: " << patch_string << endl;

    RONtStream reader{Stream::CLOSED, Stream::TMP};
    reader.FeedString(patch_string);

    Op op;
    while (reader.DrainOp(op).Ok()) {
        patch.push_back(op);
    }
}


void HandlePush(int client_socket) {
    uint64_t buffer;

    recv(client_socket, &buffer, sizeof(buffer), 0);
    cout << "Got number: " << buffer << endl;
}


void HandlePull(int client_socket) {

}


void ServeClient(int client_socket) {
    cout << "Awaiting client response" << endl;

    auto cmd = ReadCommand(client_socket);
    cout << "Client's command: " + cmd << endl;

    if(cmd == "push") {
        HandlePush(client_socket);
    } else if (cmd == "pull") {
        HandlePull(client_socket);
    }

    //bytes_written += send(client_socket, (char*)&msg, strlen(msg), 0);

    close(client_socket);
    cout << "Connection closed" << endl;
}


void Run(int port) {
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

    while (true) {
        listen(server_socket, 5);

        sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);

        int client_socket = accept(server_socket, (sockaddr *)&client_address, &client_address_size);
        if(client_socket < 0) {
            cerr << "Error accepting request from client" << endl;
            exit(1);
        }
        cout << "Connected with client" << endl;

        ServeClient(client_socket);
    }

    close(server_socket);
}

