#ifndef __ETIME_H__
#define __ETIME_H__
#include "types.h"

struct time_operations{
    void (*udelay)(e_uint32_t us);
};


#define time_before(unknow, know)    ((e_int32_t)(know)-(e_int32_t)(unknow)>0)
#define time_after(unknow, know)     time_before(know, unknow)
#define time_before_eq(unknow, know)  ((e_int32_t)(know)-(e_int32_t)(unkonw)>=0)
#define time_after_eq(unknow, know)   time_before_eq(know, unknow)

e_err_t time_init(struct time_operations ops);
void system_time_inc(void);
e_uint32_t system_get_time(void);
void bare_delay_ms(e_uint32_t ms);
void bare_delay_us(e_uint32_t us);
void delay_ms(e_uint32_t ms);
void delay_us(e_uint32_t us);
#endif