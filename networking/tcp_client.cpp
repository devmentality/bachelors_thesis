#include "tcp_client.h"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <netdb.h>

using namespace std;


void run_push(const string& server_ip, int server_port) {
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
        cout<< "Error connecting to socket!"<< endl;
        return;
    }
    cout << "Connected to the server!" << endl;

    const char cmd[] = "push";
    send(client_socket, &cmd, sizeof cmd, 0);
    uint64_t number = 42;
    send(client_socket, &number, sizeof number, 0);

    close(client_socket);

    cout << "Connection closed" << endl;
}
