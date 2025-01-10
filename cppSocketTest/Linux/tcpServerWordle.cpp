#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string>
#include <errno.h>
#include <vector>
#include <random>
#include <ctime>
#include <functional>
#include <thread>

using namespace std;

int socketThread(sockaddr_in client, socklen_t clientSize, int clientSocket, int threadNumber){

    // Which thread are you?
    cout << "I am thread " << threadNumber << endl;

    // Initialize the variables required for the wordle game
    // Initialize words in game
    vector<string> words = {"TEXAS","CHAIR", "ROBIN", "SPACE", "PHONE", "RADIO", "FRAUD", "STEAM", "ALTER", "TOWER"};
    string wordle = "_____";
    bool completed;

    // Accept a call
    char host[NI_MAXHOST];
    char svc[NI_MAXSERV];

    memset(host, 0, NI_MAXHOST);
    memset(svc, 0, NI_MAXSERV);

    int result = getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, svc, NI_MAXSERV, 0);
    if (result){
        cout << host << " connected on " << svc << endl;
    }
    else{
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout << host <<  " connected on " << ntohs(client.sin_port) << endl;
    }

    // Generate the random number generater
    mt19937 gen(time(0)); // This will generate the random number variable "gen" with a input seed of time to make it really random
    uniform_int_distribution<> distrib(0,9);

    // Update key word you are trying to guess
    // Generate the random number
    int randomValue = distrib(gen);
    string keyWord = words[randomValue];

    // While receiving display the message and echo
    char buf[4096];
    while (true){
        // Clear the buffer
        memset(buf, 0, 4096);

        // Wait for a message
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == -1){
            cerr << "There was a connection issue" << endl;
            break;
        }

        if (bytesReceived == 0){
            cout << "The client disconnected" << endl;
            break;
        } else if (bytesReceived == 6){
            // Check to see if the keyWord matches
            if (buf == keyWord){
                wordle = keyWord;
                completed = true;
            } else {
                for (int i=0; i<6; i++){
                    for (int y=i; y<6; y++){
                        if (tolower(buf[i]) == tolower(keyWord[y])){
                            if (i == y){
                                // The letter and location is correct
                                cout << buf[i] << " is correct" << endl;
                                wordle[i] = toupper(buf[i]);
                            } else {
                                // The letter is correct but wrong location
                                cout << buf[i] << " is correct but wrong place" << endl;
                                if (wordle[i] != toupper(keyWord[y])){
                                    // Checking to see if the number is already uppercase
                                    wordle[i] = tolower(buf[i]);
                                }
                            }
                        }
                    }
                }
            }
        }


        // Display the message 
        cout << "Received: " << string(buf, 0, bytesReceived) << endl;

        // Resend the message
        send(clientSocket, wordle.c_str(), 6, 0);

        if (completed){
            break;
        }
    }

    // Close the socket
    close(clientSocket);
    cout << "Socket is closed" << endl;
    return 0;
}

int main(){

    while (true){
        int i = 1;

        // Create the socket 
        int listening = socket(AF_INET, SOCK_STREAM, 0);
        if (listening == -1){
            cerr << "Can't create a socket!";
            return -1;
        }

        // Bind the socket to ip and port
        sockaddr_in hint;
        hint.sin_family = AF_INET;
        hint.sin_port = htons(54000+i); // Host to network short
        inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr); // Converts a number to an array of integers ex. 127.0.0.1

        if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1){
            cerr << "Can't bind to IP port!";
            return -2;
        }

        // Mark the socket for listening
        if (listen(listening, SOMAXCONN) == -1){
            cerr << "Can't listen!";
            return -3;
        }

        sockaddr_in client;
        socklen_t clientSize = sizeof(client);

        // Wait for a socket connection to come through and once accepted create a thread to handle that socket connection
        cout << "Trying to accept socket connection." << endl;
        int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
        if (clientSocket == -1){
            cerr << "Can't accept the socket!";
            return -4;
        }

        std::thread socketThreadServer(socketThread, client, clientSize, clientSocket, ref(i));
        socketThreadServer.join();

        i++;
    }

    return 0;
}