#include "Fifo.h"

Int8 FifoCreate(Fifo *pFifo,UInt8 *pBuf,UInt16 nFifoSize)
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

Int8 FifoIsEmpty(Fifo* pFifo)
{
	if(pFifo == NULL)
		return 0;
	if((pFifo->iFront == pFifo->iRear) && (pFifo->nSize == 0))
		return 1;
	return 0;
}

Int8 FifoIsFull(Fifo* pFifo)
{
	if(pFifo == NULL)
		return 1;
	if((pFifo->iFront == pFifo->iRear) && (pFifo->nSize == pFifo->nCapacity))
		return 1;
	return 0;
}

void FifoPush(Fifo* pFifo, UInt8 bData)
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

UInt8 FifoPop(Fifo* pFifo)
{
	UInt8 bData;
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

UInt16 FifoQueryPushed(Fifo *pFifo)
{
    return pFifo->nSize;
}

void FifoReset(Fifo *pFifo)
{
	pFifo->nSize = 0;
	pFifo->iFront = 0;
	pFifo->iRear = 0;   
}