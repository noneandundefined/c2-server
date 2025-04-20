#ifndef С2_IPGEO_H
#define С2_IPGEO_H

#include <stdint.h>

#define MAX_LINE_LENGTH 1024
#define MAX_RESPONSE_LENGTH 4096

char* ipgeo();

int get_mac_address(uint8_t mac_address[6]);

#endif
