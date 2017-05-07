#ifndef _CRC16_H
#define _CRC16_H
#include "includes.h"

UInt16 Crc16Cal(const UInt8 *buf, int len);


Int8 Crc16CheckSum(UInt16 tcrc,const UInt8 *buf, int sz);
#endif


