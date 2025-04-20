#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>

#include "c2-bot.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 4096

void BotConnection() {
    WSADATA wsaData;
    SOCKET sock;

    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        return;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Error creating socket. Code: %d\n", WSAGetLastError());
        WSACleanup();
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Failed to connect -> server. Code %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return;
    }

    printf("Connect to server %s:%d success!\n", SERVER_IP, SERVER_PORT);

    while (1) {
        // EXAMPLE SENT DATA
        const char *message = "Hello, server!";
        send(sock, message, strlen(message), 0);

        bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            perror("Failed to get bytes data");
            closesocket(sock);
            break;
        }

        buffer[bytes_received] = '\0';
        printf("Server answer: %s\n", buffer);
    }

    closesocket(sock);
    WSACleanup();
}
