#ifndef __CC_H__
#define __CC_H__
#include <stdio.h>

#ifndef	BYTE_ORDER
#define BYTE_ORDER	LITTLE_ENDIAN
#endif

#define LWIP_NO_STDINT_H    1
typedef	unsigned char 	u8_t;
typedef signed char		s8_t;
typedef unsigned short		u16_t;
typedef signed short		s16_t;
typedef unsigned long		u32_t;
typedef signed long		s32_t;
typedef u32_t	mem_ptr_t;

#if	defined (__ICCARM__)
#define PACK_STRUCT_BEGIN __packed
//#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x)	x
#define PACK_STRUCT_USE_INCLUDES

#elif defined(__CC_ARM)

#define PACK_STRUCT_BEGIN	__packed
#define PACK_STRUCT_STRUCT
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x)	x

#elif defined(__GNUC__)

#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT	__attribute__((__packed__))
#define PACK_STRUCT_END
#define PACK_STRUCT_FIELD(x)	x

#elif defined(__TASKING__)
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_STRUCT
#define PACK_STURCT_END
#define PACK_STURCT_FIELD(x)	x
#endif

#define U16_F "4d"
#define S16_F "4d"
#define X16_F "4x"
#define U32_F "8ld"
#define S32_F "8ld"
#define X32_F "8lx"

#ifndef	LWIP_PLATFORM_ASSERT
#define	LWIP_PLATFORM_ASSERT(x)	{printf(x);}
#endif

#ifndef	LWIP_PLATFORM_DIAG
#define	LWIP_PLATFORM_DIAG(x)		{printf(x);}
#endif
#endif