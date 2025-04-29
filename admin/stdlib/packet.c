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
