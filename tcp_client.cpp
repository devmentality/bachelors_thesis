#include "tcp_client.h"

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include "version_vector.h"
#include "ron/op.hpp"
#include "ron/ron-streams.hpp"
#include "socket_io.h"

using namespace std;
using namespace ron;


int Connect(const string& server_ip, int server_port) {
    struct hostent* host = gethostbyname(server_ip.c_str());

    sockaddr_in server_address;
    bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    server_address.sin_port = htons(server_port);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    int status = connect(client_socket, (sockaddr*) &server_address, sizeof(server_address));
    if(status < 0)
    {
        cout<< "Error connecting to socket"<< endl;
        return -1;
    }

    return client_socket;
}


void SendCmd(int socket, const string& cmd) {
    send(socket, cmd.c_str(), cmd.length() + 1, 0);
}


int64_t FetchOndx(int socket, uint64_t replica_id) {
    send(socket, &replica_id, sizeof replica_id, 0);
    int64_t ondx;
    recv(socket, &ondx, sizeof ondx, 0);
    return ondx;
}


void PushChanges(
        int socket,
        const map<uint64_t, Version>& version_vector,
        const vector<Op>& patch
) {
    SendVersionVector(socket, version_vector);
    SendPatch(socket, patch);

    close(socket);

    cout << "Connection closed" << endl;
}
