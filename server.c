#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>    // Windows-specific sockets library
#include <ws2tcpip.h>    // For inet_pton and other functions

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Initialize WinSock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Socket creation error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    // Bind the socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) == SOCKET_ERROR) {
        printf("Listen failed. Error Code: %d\n", WSAGetLastError());
        closesocket(server_fd);
        WSACleanup();
        return 1;
    }

    printf("Server listening on port %d\n", PORT);

    // Accept and handle incoming connections
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) == INVALID_SOCKET) {
            printf("Accept failed. Error Code: %d\n", WSAGetLastError());
            closesocket(server_fd);
            WSACleanup();
            return 1;
        }

        int read_size;
        while ((read_size = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0) {
            printf("Received: %s\n", buffer);
            send(new_socket, buffer, read_size, 0);  // Echo the message back
            memset(buffer, 0, BUFFER_SIZE);  // Clear the buffer
        }

        closesocket(new_socket);
        printf("Connection closed.\n");
    }

    closesocket(server_fd);
    WSACleanup();  // Clean up WinSock
    return 0;
}
