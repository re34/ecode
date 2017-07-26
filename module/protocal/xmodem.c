#include "xmodem.h"
#include <string.h>
#include "ecode.h"

/*
 * XMODEM protocol keywords
 */
#define XMODEM_SOH					0x01
#define XMODEM_STX					0x02
#define XMODEM_EOT					0x04
#define XMODEM_ACK					0x06
#define XMODEM_NAK					0x15
#define XMODEM_CAN					0x18
#define XMODEM_CTRLZ				0x1A

#define XMODEM_START				'C'

#define XMODEM_RETRY_TIMES	32

XmodemDescription_t fXmodemDescription;



static e_uint8_t aXmodemReceiveBuffer[1030] = {0};
//static e_int8_t blfgetcError = 1;



void XmodemInit(XmodemDescription_t desc)
{
    fXmodemDescription = desc;
}

/*
static e_uint8_t Xmodemfgetc(void)
{
	int pChar;
	e_uint32_t timeout = 0;

	while (1)
	{
        pChar = fXmodemDescription.fgetc();
		if(pChar<0)
		{
			timeout++;
			if(timeout == 0x600000) //1s
			{
				return -1;
			}
		}
        else
          break;
	}

	return pChar;
}
*/

static e_uint8_t Xmodemfgetc(void)
{
	int pChar;
	e_uint32_t timeout = 0;
    e_uint32_t local_time = 0;
    
    local_time = system_get_time();

	while (1)
	{
        pChar = fXmodemDescription.fgetc();
		if(pChar<0)
		{
          if(system_get_time()-local_time>1000)//1s
            return -1;
		}
        else
          break;
	}

	return pChar;
}

static void Xmodemfputc(e_uint8_t bData)
{
    fXmodemDescription.fputc(bData);
}

static e_int8_t XmodemCheckData(e_int8_t blUseCrc16, e_uint8_t* pData, e_uint16_t nLength)
{
	if(blUseCrc16)
	{
		e_uint16_t hCalCrc = Crc16Cal((e_uint8_t*)pData, nLength);
		e_uint16_t hTarCrc = ((e_uint16_t)pData[nLength] << 8) + pData[nLength + 1];
		if(hCalCrc != hTarCrc)
		{
			return 0;
		}
	}
	else
	{
		e_uint16_t iCount;
		e_uint8_t hChecksum = 0;

		for(iCount = 0; iCount < nLength; ++iCount)
		{
			hChecksum += *(pData + iCount);
		}

		if(hChecksum != pData[nLength] )
		{
			return 0;
		}
	}

	return 1;
}

int XmodemReceive(e_uint32_t nBytesToReceive, e_uint32_t* nBytesReceived,
		e_uint32_t nAddress, XmodemWriteData fpXmodemWriteData)
{
	e_int8_t blUseStartChar = 1;
	e_int8_t blIsReceiving = 0;
	e_int8_t blUseCrc16;
	e_uint16_t iRetry;
	int bRcvByte;
	e_uint8_t iPackIndex = 1;
	e_uint32_t nRcvBytes = 0;
	e_uint16_t iDataIndex = 0;
	e_uint16_t nPackSize;
	e_uint8_t nTempComplement;

	e_uint8_t* pBufPtr = NULL;

	e_uint8_t bStartByte = XMODEM_START;

	//e_uint8_t iCount;
	if(fpXmodemWriteData == NULL)
	{
		LOG_ERROR("Xmodem description is not correct!Maybe write function doesn't setup!");
		return -1;
	}

	memset(aXmodemReceiveBuffer, 0, sizeof(aXmodemReceiveBuffer));

	while(1)
	{

		for(iRetry = 0; iRetry < XMODEM_RETRY_TIMES; iRetry++)
		{
			if(blIsReceiving == 0)
			{
				Xmodemfputc(bStartByte);
			}

			bRcvByte = Xmodemfgetc();
			if(bRcvByte<0)
			{
				continue;
			}

			switch(bRcvByte)
			{
				case XMODEM_SOH:
					nPackSize = 128;
					goto XmodemReceiveLable;
				case XMODEM_STX:
					nPackSize = 1024;
					goto XmodemReceiveLable;
				case XMODEM_EOT:
					Xmodemfputc(XMODEM_ACK);
					*nBytesReceived = nRcvBytes;
					return 0;
				case XMODEM_CAN:
					bRcvByte = Xmodemfgetc();
					if(bRcvByte == XMODEM_CAN)
					{
						Xmodemfputc(XMODEM_ACK);
						return -1;
					}
					break;
				default:
					break;
			}
		}

		Xmodemfputc(XMODEM_CAN);
		Xmodemfputc(XMODEM_CAN);
		Xmodemfputc(XMODEM_CAN);
		return -1;

XmodemReceiveLable:
		blIsReceiving = 1;

		if(blUseStartChar)
			blUseCrc16 = 1;
		else
			blUseCrc16 = 0;

		pBufPtr = aXmodemReceiveBuffer;
		*pBufPtr++ = bRcvByte;
		for(iDataIndex = 0; iDataIndex < (nPackSize + (blUseCrc16?1:0) + 3); ++iDataIndex)
		{
			bRcvByte = Xmodemfgetc();
			if(bRcvByte<0)
			{
				goto XmodemRejectLable;
			}
			*pBufPtr++ = bRcvByte;
		}

		nTempComplement  = ~aXmodemReceiveBuffer[2];
		if((aXmodemReceiveBuffer[1] == nTempComplement) &&
				((aXmodemReceiveBuffer[1] == iPackIndex) || (aXmodemReceiveBuffer[1] == (iPackIndex - 1))) &&
				(XmodemCheckData(blUseCrc16, &aXmodemReceiveBuffer[3], nPackSize)))
		{
			if(aXmodemReceiveBuffer[1] == iPackIndex)
			{
				e_uint32_t nPayloadLength;
				nPayloadLength = nBytesToReceive - nRcvBytes;

				if(nPayloadLength > nPackSize)
				{
					nPayloadLength = nPackSize;
				}
				if(nPayloadLength > 0)
				{

					if(fpXmodemWriteData(nAddress, &aXmodemReceiveBuffer[3], nPayloadLength) == 0)
					{
						nAddress += nPayloadLength;
						nRcvBytes += nPayloadLength;
					}
					else
					{
						return -1;
					}
				}
				++iPackIndex;
			}

			Xmodemfputc(XMODEM_ACK);
			continue;
		}

XmodemRejectLable:
		Xmodemfputc(XMODEM_NAK);
	}

	//return 0;
}
