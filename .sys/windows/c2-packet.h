#ifndef C2_PACKET_H
#define C2_PACKET_H

#include <stdint.h>
#include <winsock2.h>

#pragma pack(push, 1)
typedef struct {
    uint8_t mac[6];
    uint8_t version;
    uint8_t type;
    uint16_t ipgeo_length;
} HelloHeaderPacket;
#pragma pack(pop)

int hello_packet(SOCKET sock, const uint8_t* mac, const char* ipgeo_json);

int keep_alive(SOCKET sock);

#endif
