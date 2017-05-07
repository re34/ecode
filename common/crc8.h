#ifndef _CRC8_H
#define _CRC8_H
#include <stdint.h>
#include "includes.h"


UInt8 Crc8Ccitt(UInt8 *buf, int len);


int Crc8Check(UInt8 tcrc,UInt8 *buf, int sz);

#endif
