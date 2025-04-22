#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#include "ICU_packet.h"

#pragma comment(lib, "ws2_32.lib")

int hello_packet(SOCKET sock, const uint8_t* mac, const char* ipgeo_json) {
    HelloHeaderPacket header;
    memcpy(header.mac, mac, 6);
    header.version = 0x01;
    header.type = 0x01;
    header.ipgeo_length = (uint16_t)strlen(ipgeo_json);

    size_t total_size = sizeof(HelloHeaderPacket) + header.ipgeo_length;
    uint8_t* buffer = malloc(total_size);

    memcpy(buffer, &header, sizeof(HelloHeaderPacket));
    memcpy(buffer + sizeof(HelloHeaderPacket), ipgeo_json, header.ipgeo_length);

    int sent = send(sock, (const char*)buffer, (int)total_size, 0);
    free(buffer);
    return sent;
}

int keep_alive(SOCKET sock) {
    unsigned char keep_alive_packet = 0xAA;
    size_t sent = send(sock, (const char*)&keep_alive_packet, sizeof(keep_alive_packet), 0);

    return sent;
}
