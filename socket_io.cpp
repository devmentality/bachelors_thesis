#include "socket_io.h"

#include <iostream>
#include <map>
#include "version_vector.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include "ron/ron-streams.hpp"
#include "ron/op.hpp"

using namespace std;
using namespace ron;


void ReadVersionVector(map<uint64_t, Version>& remote_version_vector, int client_socket) {
    int items_count;

    recv(client_socket, &items_count, sizeof items_count, 0);
    //cout << "VV rows count: " << items_count << endl;

    for(auto i = 0; i < items_count; i++) {
        uint64_t replica_id;
        int64_t clock;

        recv(client_socket, &replica_id, sizeof replica_id, 0);
        recv(client_socket, &clock, sizeof clock, 0);

        //cout << replica_id << " -> " << clock << endl;

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
    //cout << "PATCH: " << patch_string << endl;

    RONtStream reader{Stream::CLOSED, Stream::TMP};
    reader.FeedString(patch_string);

    Op op;
    while (reader.DrainOp(op).Ok()) {
        patch.push_back(op);
    }
}



void SendVersionVector(
        int client_socket,
        const map<uint64_t, Version>& version_vector
) {
    int items_count = version_vector.size();
    send(client_socket, &items_count, sizeof items_count, 0);

    for(auto item: version_vector) {
        auto replica_id = item.first;
        auto clock = item.second.clock;
        send(client_socket, &replica_id, sizeof replica_id, 0);
        send(client_socket, &clock, sizeof clock, 0);
    }
}


void SendPatch(
        int client_socket,
        const vector<Op>& patch
) {
    RONtStream patch_stream{Stream::CLOSED, Stream::TMP};
    for(const auto &op: patch) {
        patch_stream.FeedOp(op);
    }
    patch_stream.FeedEOF();

    auto serialized_patch = as_string(patch_stream.Filled());
    auto serialized_patch_size = serialized_patch.length();

    send(client_socket, &serialized_patch_size, sizeof serialized_patch_size, 0);
    send(client_socket, serialized_patch.c_str(), serialized_patch_size, 0);
}