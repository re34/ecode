#include "ecode_tick.h"
#include "target.h"

#define TICKS_PER_SECOND    (1000)


UInt32 _ecode_ticks=0;
tick_callback_t ftick_callback = NULL;
LIST_HEAD(timeout_head);


void ecode_tick_init(void)
{
#if RTOS_EN==0
    target_tick_init(TICKS_PER_SECOND, 1);
#endif
    _ecode_ticks = 0;
}

void ecode_tick_set_callback(tick_callback_t callback)
{
    ftick_callback = callback;
}


void ecode_tick_attach_timeout(struct timeout *timeout, timeout_callback_t callback,int ms)
{
    timeout->base = _ecode_ticks+ms;
    timeout->callback = callback;
    list_add(&timeout_head, &timeout->entry);
}

void ecode_timeout_polling(void)
{
    struct list_head *node=NULL;
    struct list_head *tnode = NULL;
    struct timeout *_timeout=NULL;
    list_for_each_safe(node, tnode, &timeout_head)
    {
        _timeout = (struct timeout*)node;
        if(time_after(_ecode_ticks, _timeout->base))
        {
            list_del(node);
            if(_timeout->callback!=NULL)
                _timeout->callback();
        }
    }
}

void ecode_tick_inc(void)
{
    _ecode_ticks++;
    if(ftick_callback!=NULL)
    {
        ftick_callback(1);
    }
    ecode_timeout_polling();
}

UInt32 ecode_get_ticks(void)
{
    return _ecode_ticks;
}

void ecode_tick_delay_us(UInt32 us)
{
    UInt32 startTick = SysTick->VAL;
    UInt32 currTick = 0;
    UInt32 reload = SysTick->LOAD;
    UInt32 tUsTick = SysTick->LOAD/1000;
    while(us){

        currTick = SysTick->VAL;
        if(currTick>startTick){
            if(reload-currTick+startTick>=tUsTick){
                us--;
                startTick = currTick;
            }
        }else{
            if(startTick-currTick>=tUsTick){
                us--;
                startTick = currTick;
            }			
        }
    }
}

void ecode_tick_delay_ms(UInt32 ms)
{
    while(ms--)
    {
        ecode_tick_delay_us(1000);
    }
}

#if RTOS_EN==0
void SysTick_Handler(void)
{
    ecode_tick_inc();
}
#endif



