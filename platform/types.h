#ifndef __TYPES_H__
#define __TYPES_H__
#include "config.h"


typedef unsigned char UInt8;
typedef signed char Int8;
typedef unsigned short UInt16;
typedef signed short Int16;
typedef unsigned long UInt32;
typedef signed long Int32;

#define BIT(b)	(1U<<(b))

#define BIT_SET(x,b)	((x)|=BIT(b))
#define BIT_RESET(x,b)	((x)&=~(BIT(b)))
#define BITS_SET(x,bits)	((x)|=bits)
#define BITS_RESET(x,bits)	((x)&=~(bi/bits))
#define TEST_BIT(x,b)		(((x)&(BIT(b)))?1:0)
#define TEST_BITS(x,bits)	(((x)&(bits))?1:0)


#ifndef NULL
#define NULL	0
#endif

enum{
	BIT_LOW=0,
	BIT_HIGH=!BIT_LOW,
};

#endif
