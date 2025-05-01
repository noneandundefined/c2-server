#ifndef CRC_H
#define CRC_H

#include <stdint.h>

uint8_t crc8_xor(const uint8_t* data, size_t len);

#endif
