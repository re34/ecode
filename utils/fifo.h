#ifndef _FIFO_H_
#define _FIFO_H_
#include "includes.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	UInt8* pBuffer;
	UInt16 iFront;
	UInt16 iRear;
	UInt16 nSize;
	UInt16 nCapacity;
}Fifo;


Int8 FifoCreate(Fifo *pFifo,UInt8 *pBuf,UInt16 nFifoSize);
void FifoFinish(Fifo* pFifo);

Int8 FifoIsEmpty(Fifo* pFifo);
Int8 FifoIsFull(Fifo* pFifo);

void FifoPush(Fifo* pFifo, UInt8 bData);
UInt8 FifoPop(Fifo* pFifo);

UInt16 FifoQueryPushed(Fifo *pFifo);
void FifoReset(Fifo *pFifo);
#ifdef __cplusplus
}
#endif

#endif /* _FIFO_H_ */
