#include <iostream>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>

using namespace std;

int main(){

    // Create the TCP client
    string ipAddress = "127.0.0.1";
    int port = 54000;

    // Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);
    int wsOk = WSAStartup(ver, &wsData);

    if (wsOk != 0){
        cerr << "Can't initialize winsock! Quitting" << endl;
        return 0;
    }

    // Create the socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET){
        cerr << "Can't create a socket! Quitting." << endl;
        WSACleanup();
        return 0;
    }

    // Fill in hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    // Connect to the server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR){
        cerr << "Can't connect to server! Quitting." << endl;
        closesocket(sock);
        WSACleanup();
        return 0;
    }

    // Do-while loop to send and receive data
    char buf[4096];
    string userInput = "";

    do {
        // Promt the user for some text
        cout << "> .";
        getline(cin, userInput);

        // Send the text
        if (userInput.size() > 0) { // Make sure the user typed in something
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR) {
                // Wait for response
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0){
                    // Echo response to console
                    cout << "Server>" << string(buf, 0, bytesReceived) << endl;
                }
            }
        }

    } while (userInput.size() > 0);

    // Clean up winsock
    closesocket(sock);
    WSACleanup();

}