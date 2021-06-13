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


void RunPush(
        const string& server_ip,
        int server_port,
        const map<uint64_t, Version>& version_vector,
        const vector<Op>& patch
) {
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
        return;
    }
    cout << "Connected to the server" << endl;

    const char cmd[] = "push";
    send(client_socket, &cmd, sizeof cmd, 0);
    SendVersionVector(client_socket, version_vector);
    SendPatch(client_socket, patch);

    close(client_socket);

    cout << "Connection closed" << endl;
}
