#include <WinSock2.h>
#include <ws2tcpip.h>
#include "iostream"
#include <tchar.h>

#pragma comment(lib, "ws2_32")

using namespace std;

int main(int argc, char* argv[]){

    cout << "===== Step 1 - Set up DLL =====" << endl;

    SOCKET serverSocket, acceptSocket;
    int port = 55555;
    WSADATA wsaData;
    int wsaerr;
    WORD wVersionRequested = MAKEWORD(2, 2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if (wsaerr != 0){
        cout << "The Winsock dll not found!" << endl;
        return 0;
    }
    else{
        cout << "The Winsock dll found!" << endl;
        cout << "The Status." << wsaData.szSystemStatus << endl;
    }

    cout << "===== Step 2 - Set up server socket =====" << endl;

    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (serverSocket == INVALID_SOCKET){
        cout << "Error at socket(): " << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }
    else{
        cout << "socket() is OK!" << endl;
    }

    cout << "===== Step 3 - Bind the socket =====" << endl;
    
    sockaddr_in service;
    service.sin_family = AF_INET;
    InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
    service.sin_port = htons(55555);
    if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR){
        cout << "bind() failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else{
        cout << "bind() is OK!" << endl;
    }

    cout << "===== Step 4 - Listen to socket =====" << endl;

    if (listen(serverSocket, 1)==SOCKET_ERROR){
        cout << "Listen() Error listening on socket " << WSAGetLastError() << endl;
        return 0;
    }
    else{
        cout << "Listen() is OK!, I'm waiting on connections..." << endl;
    }

    cout << "===== Step 5 - Accept connection" << endl;
    acceptSocket = accept(serverSocket,NULL,NULL);
    if (acceptSocket==INVALID_SOCKET){
        cout << "Accept failed!: " << WSAGetLastError() << endl;
        WSACleanup();
        return -1;
    }

    cout << "Accepted connection" << endl;
    system("pause");
    WSACleanup();

    return 0;
}
