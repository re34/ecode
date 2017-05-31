#ifndef __TICKS_H__
#define __TICKS_H__
#include "includes.h"


#define time_before(unknow, know)    ((Int32)(know)-(Int32)(unknow)>0)
#define time_after(unknow, know)     time_before(know, unknow)
#define time_before_eq(unknow, know)  ((Int32)(know)-(Int32)(unkonw)>=0)
#define time_after_eq(unknow, know)   time_before_eq(know, unknow)

typedef void (*tick_callback_t)(int ms);


typedef void (*timeout_callback_t)(void);
/*
struct timeout{
    struct list_head entry;
    UInt32 base;
    timeout_callback_t callback;
};*/

void tick_set_callback(tick_callback_t callback);
//void tick_attach_timeout(struct timeout *timeout, timeout_callback_t callback,int ms);
void tick_inc(void);
UInt32 ecode_get_ticks(void);
//void tick_delay_us(UInt32 us);
void tick_delay_ms(UInt32 ms);
#endif