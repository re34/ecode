#ifndef _XMODEM_H_
#define _XMODEM_H_

#include"types.h"


typedef struct{
    int (*fgetc)(void);
    void (*fputc)(e_uint8_t data);
}XmodemDescription_t;


typedef int (*XmodemWriteData)(e_uint32_t nAddress, e_uint8_t* pData, e_uint16_t nLength);

void XmodemInit(XmodemDescription_t desc);

int XmodemReceive(e_uint32_t nBytesToReceive, e_uint32_t* nBytesReceived,
		e_uint32_t nAddress, XmodemWriteData fpSeXmodemWriteData);

#endif /* _XMODEM_H_ */
