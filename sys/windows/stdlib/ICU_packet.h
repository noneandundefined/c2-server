#ifndef ICU_PACKET_H
#define ICU_PACKET_H

#include <stdint.h>
#include <winsock2.h>

#pragma pack(push, 1)
typedef struct {
    uint16_t size;
    uint8_t version;
    uint8_t type;
    uint8_t mac[6];
    uint16_t ipgeo_length;
    uint8_t crc;
} HelloPacket;
#pragma pack(pop)

int hello_packet(SOCKET sock, const uint8_t* mac, const char* ipgeo_json);

int keep_alive(SOCKET sock);

#endif
