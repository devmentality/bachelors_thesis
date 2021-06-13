#include "tcp_client.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <sys/uio.h>
#include <ctime>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <string>
using namespace std;





void run_push(const string& server_ip, int server_port) {
    struct hostent* host = gethostbyname(server_ip.c_str());

    sockaddr_in sendSockAddr;
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(server_port);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    int status = connect(client_socket, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        cout<<"Error connecting to socket!"<<endl; break;
    }
    cout << "Connected to the server!" << endl;

    const char "push"
    cout << ">";
    getline(cin, data);
    memset(&msg, 0, sizeof(msg));//clear the buffer
    strcpy(msg, data.c_str());
    if(data == "exit")
    {
        send(client_socket, (char*)&msg, strlen(msg), 0);
        break;
    }
    bytesWritten += send(client_socket, (char*)&msg, strlen(msg), 0);
    cout << "Awaiting server response..." << endl;
    memset(&msg, 0, sizeof(msg));//clear the buffer
    bytesRead += recv(client_socket, (char*)&msg, sizeof(msg), 0);
    if(!strcmp(msg, "exit"))
    {
        cout << "Server has quit the session" << endl;
        break;
    }
    cout << "Server: " << msg << endl;

    close(client_socket);

    cout << "Connection closed" << endl;
}
int main(int argc, char *argv[])
{
    //we need 2 things: ip address and port number, in that order
    if(argc != 3)
    {
        cerr << "Usage: ip_address port" << endl; exit(0);
    } //grab the IP address and port number
    char *serverIp = argv[1]; int port = atoi(argv[2]);
    //create a message buffer
    char msg[1500];
    //setup a socket and connection tools

}