#ifndef _FIFO_H_
#define _FIFO_H_
#include "types.h"


#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	e_uint8_t* pBuffer;
	e_uint16_t iFront;
	e_uint16_t iRear;
	e_uint16_t nSize;
	e_uint16_t nCapacity;
}Fifo;


e_int8_t FifoCreate(Fifo *pFifo,e_uint8_t *pBuf,e_uint16_t nFifoSize);
void FifoFinish(Fifo* pFifo);

e_int8_t FifoIsEmpty(Fifo* pFifo);
e_int8_t FifoIsFull(Fifo* pFifo);

void FifoPush(Fifo* pFifo, e_uint8_t bData);
e_uint8_t FifoPop(Fifo* pFifo);

e_uint16_t FifoQueryPushed(Fifo *pFifo);
void FifoReset(Fifo *pFifo);
#ifdef __cplusplus
}
#endif

#endif /* _FIFO_H_ */
