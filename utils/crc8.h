#ifndef _CRC8_H
#define _CRC8_H
#include <stdint.h>
#include "types.h"


e_uint8_t Crc8Ccitt(e_uint8_t *buf, int len);


int Crc8Check(e_uint8_t tcrc,e_uint8_t *buf, int sz);

#endif
