#ifndef __ECODE_ASSERT_H__
#define __ECODE_ASSERT_H__
#include "types.h"
#include "config.h"


#ifdef __cplusplus
 extern "C" {
#endif

#if CONFIG_USE_ASSERT==1
    #define ecode_assert(expr)  ((expr)?0U:ecode_assert_failed((e_uint8_t *)__FILE__, __LINE__))
    void ecode_assert_failed(e_uint8_t *file, e_uint32_t line);
#else
    #define ecode_assert(expr)  ((void)0)
#endif


#ifdef __cplusplus
}
#endif

#endif