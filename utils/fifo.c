#include "Fifo.h"

e_int8_t FifoCreate(Fifo *pFifo,e_uint8_t *pBuf,e_uint16_t nFifoSize)
{
	if(pBuf == NULL)
		return -1;

	if(pFifo == NULL)
	{
		return -2;
	}

	pFifo->pBuffer = pBuf;
	pFifo->nSize = 0;
	pFifo->nCapacity = nFifoSize;
	pFifo->iFront = 0;
	pFifo->iRear = 0;

	return 0;
}

e_int8_t FifoIsEmpty(Fifo* pFifo)
{
	if(pFifo == NULL)
		return 0;
	if((pFifo->iFront == pFifo->iRear) && (pFifo->nSize == 0))
		return 1;
	return 0;
}

e_int8_t FifoIsFull(Fifo* pFifo)
{
	if(pFifo == NULL)
		return 1;
	if((pFifo->iFront == pFifo->iRear) && (pFifo->nSize == pFifo->nCapacity))
		return 1;
	return 0;
}

void FifoPush(Fifo* pFifo, e_uint8_t bData)
{
	*(pFifo->pBuffer + pFifo->iRear) = bData;
	if((pFifo->iRear + 1) == pFifo->nCapacity)
	{
		pFifo->iRear = 0;
	}else{
		pFifo->iRear++;
	}
	pFifo->nSize++;

	return;
}

e_uint8_t FifoPop(Fifo* pFifo)
{
	e_uint8_t bData;
	bData = *(pFifo->pBuffer + pFifo->iFront);
	if((pFifo->iFront + 1) == pFifo->nCapacity)
	{
		pFifo->iFront = 0;
	}else{
		pFifo->iFront++;
	}
	pFifo->nSize--;

	return bData;
}

e_uint16_t FifoQueryPushed(Fifo *pFifo)
{
    return pFifo->nSize;
}

void FifoReset(Fifo *pFifo)
{
	pFifo->nSize = 0;
	pFifo->iFront = 0;
	pFifo->iRear = 0;   
}