#include <iostream>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>

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
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton ...

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
    char buf[1];
    string keyWord = "Hello";
    char wordle[6] = "_____";

    while (true){
        ZeroMemory(buf, 1);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 1, 0);
        if (bytesReceived == SOCKET_ERROR){
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }

        if (bytesReceived == 0){
            cout << "Client disconnected " << endl;
            break;
        }

        // Update key word you are trying to guess

        cout << "Checking if buff is null" << endl;
        // Echo message back to client
        if (buf[0] == '\0') {
            cout << "Buff is null" << endl;
            send(clientSocket, "The guess must be one character", 31, 0);
        } else {
            cout << "Checking if buf in keyword" << endl;
            if (keyWord.find(buf[0]) != string::npos){
                cout << "Buf in keyword" << keyWord.length() << endl;
                for (int i=0; i < keyWord.length() + 1; i++){
                    if (keyWord[i] == buf[0]) {
                        cout << keyWord[i] << endl;
                        cout << buf[0] << endl;
                        wordle[i] = buf[0];
                    }
                }
            }
        }
        cout << wordle << endl;
        send(clientSocket, wordle, 6, 0);
    }

    // Close the socket
    closesocket(clientSocket);

    // Cleanup winsock
    WSACleanup();

    return 0;
}