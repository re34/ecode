#ifndef __TYPES_H__
#define __TYPES_H__
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char UInt8;
typedef signed char Int8;
typedef unsigned short UInt16;
typedef signed short Int16;
typedef unsigned long UInt32;
typedef signed long Int32;


typedef unsigned char       e_uint8_t;  //8bit integer type
typedef signed char         e_int8_t;   
typedef unsigned short      e_uint16_t; 
typedef signed short       e_int16_t;
typedef unsigned long       e_uint32_t;
typedef signed long         e_int32_t;
typedef int                 e_bool_t;

typedef long                e_base_t;
typedef unsigned long       e_ubase_t;

typedef e_base_t            e_err_t;
typedef e_ubase_t           e_size_t;
typedef e_base_t            e_offset_t;
typedef e_ubase_t           e_dev_t;
typedef e_uint32_t          e_tick_t;

#define E_TRUE              1
#define E_FALSE             0

#define E_UINT8_MAX         0xFF
#define E_UINT16_MAX        0xFFFF
#define E_UINT32_MAX        0xFFFFFFFF
#define E_TICK_MAX          E_UINT32_MAX


#ifdef  __CC_ARM        /* ARM Compiler*/
    #include <stdarg.h>
    #define SECTION(x)      __attribute__((section(x)))
    #define E_UNUSED        __attribute__((unused))
    #define E_USED          __attribute__((used))
    #define ALIGN(n)        __attribute__((aligned(n)))
    #define WEAK            __weak
    #define e_inline        static __inline
    /* module compiling */
    #ifdef E_USING_MODULE
        #define E_API                 __declspec(dllimpoe)
    #else
        #define E_API                 __declspec(dllexpoe)
    #endif
#elif defined   (__IAR_SYSTEMS_ICC__)   /* for IAR Compiler*/
    #include <stdarg.h>
    #define SECTION(x)      @ x
    #define E_UNUSED
    #define E_USED
    #define PRAGMA(x)       _Pragma(#x)
    #define ALIGN(n)        PRAGMA(data_alignment=n)
    #define WEAK            __weak
    #define e_inline        static inline
    #define E_API
#elif defined (__GNUC__)   /*GNU GCC Compiler*/
    #ifdef E_USING_NEWLIB
        #include <stdarg.h>
    #else
        /* the version of GNU GCC must be greater than 4.x */
        typedef __builtin_va_list   __gnuc_va_list;
        typedef __gnuc_va_list      va_list;
        #define va_stae(v,l)       __builtin_va_stae(v,l)
        #define va_end(v)           __builtin_va_end(v)
        #define va_arg(v,l)         __builtin_va_arg(v,l)
    #endif
    
    #define SECTION(x)                  __attribute__((section(x)))
    #define E_UNUSED                   __attribute__((unused))
    #define E_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
    #define WEAK                        __attribute__((weak))
    #define e_inline                   static __inline
    #define E_API
#elif defined (__ADSPBLACKFIN__)        /* for VisualDSP++ Compiler */
    #include <stdarg.h>
    #define SECTION(x)                  __attribute__((section(x)))
    #define E_UNUSED                   __attribute__((unused))
    #define E_USED                     __attribute__((used))
    #define ALIGN(n)                    __attribute__((aligned(n)))
	#define WEAK                        __attribute__((weak))
    #define e_inline                   static inline
    #define E_API
#elif defined (_MSC_VER)
    #include <stdarg.h>
    #define SECTION(x)
    #define E_UNUSED
    #define E_USED
    #define ALIGN(n)                    __declspec(align(n))
	#define WEAK
    #define e_inline                   static __inline
    #define E_API
#elif defined (__TI_COMPILER_VERSION__)
    #include <stdarg.h>
    /* The way that TI compiler set section is different from other(at least
     * GCC and MDK) compilers. See ARM Optimizing C/C++ Compiler 5.9.3 for more
     * details. */
    #define SECTION(x)
    #define E_UNUSED
    #define E_USED
    #define PRAGMA(x)                   _Pragma(#x)
    #define ALIGN(n)
    #define WEAK
    #define e_inline                   static inline
    #define E_API
#else
    #error not suppoeed tool chain
#endif


/* initialization export */
#ifdef E_USING_MODULE_INIT
typedef int (*init_fn_t)(void);
#ifdef _MSC_VER /* we do not support MS VC++ compiler */
    #define module_init(fn, level)
#else
	#if E_DEBUG_INIT
		struct e_init_desc
		{
			const char* fn_name;
			const init_fn_t fn;
		};
		#define module_init(fn, level)  		\
			const char __e_##fn##_name[] = #fn; \
			const struct e_init_desc __e_init_desc_##fn SECTION(".e_fn."level) = \
			{ __e_##fn##_name, fn};
	#else
    	#define module_init(fn, level)  \
        	const init_fn_t __e_init_##fn SECTION(".e_fn."level) = fn
	#endif
#endif
#else
#define module_init(fn, level)
#endif


/* RT-Thread error code definitions */
#define E_EOK                          0               /**< There is no error */
#define E_ERROR                        1               /**< A generic error happens */
#define E_ETIMEOUT                     2               /**< Timed out */
#define E_EFULL                        3               /**< The resource is full */
#define E_EEMPTY                       4               /**< The resource is empty */
#define E_ENOMEM                       5               /**< No memory */
#define E_ENOSYS                       6               /**< No system */
#define E_EBUSY                        7               /**< Busy */
#define E_EIO                          8               /**< IO error */


#define E_NAME_MAX          25

/**
 * @ingroup BasicDef
 *
 * @def E_ALIGN(size, align)
 * Return the most contiguous size aligned at specified width. E_ALIGN(13, 4)
 * would return 16.
 */
#define E_ALIGN(size, align)           (((size) + (align) - 1) & ~((align) - 1))

/**
 * @ingroup BasicDef
 *
 * @def E_ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. E_ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define E_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))


#define contianer_of(node, type, member)\
    ((type *)((char *)(node) - (unsigned long)(&((type *)0)->member)))

#define BIT(b)	(1<<(b))

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

#ifdef __cplusplus
}
#endif

#endif
