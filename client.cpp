#include <iostream>
#include <cstdlib>
#include <cstring>
// #include <winsock2.h>

#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#define SA struct sockaddr



// Constants
const int PORT = 12345;
const int BUFFER_SIZE = 1024;

int main() {

    /*
    // Initialize Winsock
    WSADATA wsa_data;
    int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Create the client socket
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    */

    int sockfd, connfd;
    struct sockaddr_in cli;

    // Initialize server address
    struct sockaddr_in servaddr;
    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(PORT);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cout << "Fail creating client socket" << std::endl;
    }

    result = connect(client_socket, (sockaddr*)&server_address, sizeof(server_address));
    if (result == SOCKET_ERROR) {
        std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }

    // Receive the welcome message from the server
    char buffer[BUFFER_SIZE];
    result = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (result > 0) {
        std::cout << buffer;
    }

    // Wait for all players to connect
    bool game_started = false;
    while (!game_started) {
        // Wait for a message from the server
        result = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (result > 0) {
            std::cout << buffer;

            // Check if the game has started
            if (std::strstr(buffer, "Game starting") != nullptr) {
                game_started = true;
            }
        }
    }

    // Loop through the questions and answer them
    while (true) {
        // Receive the question from the server
        result = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (result > 0) {
            std::cout << buffer;
        }

        // Wait for the player to answer
        std::string answer;
        std::cout << "Your answer: ";
        std::getline(std::cin, answer);
        send(client_socket, answer.c_str(), answer.size(), 0);

        // Receive the result from the server
        result = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (result > 0) {
            std::cout << buffer;

            // Check if the game is over
            if (std::strstr(buffer, "Game over") != nullptr) {
                break;
            }
        }
    }

    // Close the client socket and cleanup Winsock
    closesocket(client_socket);
    WSACleanup();

    return 0;
}