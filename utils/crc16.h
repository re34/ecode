#ifndef _CRC16_H
#define _CRC16_H
#include "types.h"

e_uint16_t Crc16Cal(const e_uint8_t *buf, int len);


e_int8_t Crc16CheckSum(e_uint16_t tcrc,const e_uint8_t *buf, int sz);
#endif


