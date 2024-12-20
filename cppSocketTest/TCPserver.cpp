#include <iostream>
#include <ws2tcpip.h>
#include <windows.h>

using namespace std;

int main() {

    // Create the TCP server

    // Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);

    if (wsOk != 0){
        cerr << "Can't initialize winsock! Quitting" << endl;
        return 0;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET){
        cerr << "Can't create a socket! Quitting" << endl;
        return 0;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also unse inet_pton ...

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET){
        cerr << "Client socket is not valid. Stop!" << endl;
        return 0;
    }

    char host[NI_MAXHOST]; // Client remote name
    char service[NI_MAXHOST]; // Service (i.e port) the client is connected on

    ZeroMemory(host, NI_MAXHOST); // same as memset(host, o, NI_MAXHOST);
    ZeroMemory(service, NI_MAXHOST);

    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0){
        cout << host << " Connected on port " << service << endl;
    } else {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host << " Connected on port " << ntohs(client.sin_port) << endl;
    }

    // Close listening socket
    closesocket(listening);

    // While loop: accept and echo message back to client
    char buf[4096];
    string keyWord = "";

    while (true){
        ZeroMemory(buf, 4096);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR){
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }

        if (bytesReceived == 0){
            cout << "Client disconnected " << endl;
            break;
        }

        // Update key word you are trying to guess

        // Echo message back to client

        send(clientSocket, "hello", 5, 0);

    }

    // Close the socket
    closesocket(clientSocket);

    // Cleanup winsock
    WSACleanup();

    return 0;
}