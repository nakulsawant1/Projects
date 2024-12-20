#include <iostream>
#include <ws2tcpip.h>

using namespace std;

int main() {

    // Create the TCP server

    // Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);

    if (wsOk != 0){
        cerr << "Can;t initialize winsock! Quitting" << endl;
        return 0;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCKET_STREAM, 0);
    if (listening == INVALID_SOCKET){
        cerr << "Can't create a socket! Quitting" << endl;
        return 0;
    }

    // Bind the socket to an ip address and port

    // Tell winsock the socket is for listening

    // Wait for a connection

    // Close listening socket

    // While loop: accept and echo message back to client

    // Close the socket

    // Shutdown winsock

    return 0;
}