#include "tcp_server.h"

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>


using namespace std;


string read_command(int client_socket) {
    char cmd[5] = {0};
    recv(client_socket, (char*)&cmd, sizeof(cmd), 0);
    return string(cmd);
}


void handle_push(int client_socket) {
    uint64_t buffer;

    recv(client_socket, &buffer, sizeof(buffer), 0);
    cout << "Got number: " << buffer << endl;
}


void handle_pull(int client_socket) {

}


void serve_client(int client_socket) {
    cout << "Awaiting client response" << endl;

    auto cmd = read_command(client_socket);
    cout << "Client's command: " + cmd << endl;

    if(cmd == "push") {
        handle_push(client_socket);
    } else if (cmd == "pull") {
        handle_pull(client_socket);
    }

    //bytes_written += send(client_socket, (char*)&msg, strlen(msg), 0);

    close(client_socket);
    cout << "Connection closed" << endl;
}


void run(int port) {
    //setup a socket and connection tools
    sockaddr_in server_address;
    bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        return;
    }

    int bind_status = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if(bind_status < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        return;
    }
    cout << "Waiting for a client to connect..." << endl;

    while (true) {
        listen(server_socket, 5);

        sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);

        int client_socket = accept(server_socket, (sockaddr *)&client_address, &client_address_size);
        if(client_socket < 0)
        {
            cerr << "Error accepting request from client!" << endl;
            exit(1);
        }
        cout << "Connected with client!" << endl;

        serve_client(client_socket);
    }

    close(server_socket);
}

