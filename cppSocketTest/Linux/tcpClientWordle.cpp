#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;


int main(){
    // Initialize words in game
    vector<string> words = {"TEXAS","CHAIR", "ROBIN", "SPACE", "PHONE", "RADIO", "FRAUD", "STEAM", "ALTER", "TOWER"};

    // Initialize the game number
    int i = 0;
    cin >> i;

    // Create the socket 
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1){
        cerr << "Can't create a socket!";
        return -1;
    }

    // Bind the socket to ip and port
    sockaddr_in hint;
    string ipAddress = "127.0.0.1";
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000+i); // Host to network short
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr); // Converts a number to an array of integers ex. 127.0.0.1

    int connectResult = connect(sock, (sockaddr*)&hint, sizeof(hint));

    if (connectResult == -1){
        cerr << "Can't connect to IP port!";
        return -2;
    }

    char buf[4096];
    string userInput;

    do {
        // Enter line of text
        cout << ">.";
        getline(cin, userInput);

        // Send to server
        int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        if (sendResult == -1) {
            cout << "Could not send to server!" << endl;
            continue;
        }

        // Check for response
        memset(buf, 0, 4096);
        int bytesReceived = recv(sock, buf, 4096, 0);

        // Display
        cout << "Wordle: " << string(buf, bytesReceived) << endl;

        if (find(words.begin(), words.end(), buf) != words.end()){
            cout << "Congratulations you solved the wordle!!" << endl;
            break;
        }

    } while(true);

    // Close the socket
    close(sock);
    return 0;
}