#include "ticks.h"


#define TICKS_PER_SECOND    (1000)


static UInt32 _ticks=0;
tick_callback_t ftick_callback = NULL;
LIST_HEAD(timeout_head);


void tick_set_callback(tick_callback_t callback)
{
    ftick_callback = callback;
}

/*
void tick_attach_timeout(struct timeout *timeout, timeout_callback_t callback,int ms)
{
    timeout->base = _ticks+ms;
    timeout->callback = callback;
    list_add(&timeout_head, &timeout->entry);
}

void timeout_polling(void)
{
    struct list_head *node=NULL;
    struct list_head *tnode = NULL;
    struct timeout *_timeout=NULL;
    list_for_each_safe(node, tnode, &timeout_head)
    {
        _timeout = (struct timeout*)node;
        if(time_after(_ticks, _timeout->base))
        {
            list_del(node);
            if(_timeout->callback!=NULL)
                _timeout->callback();
        }
    }
}
*/
void tick_inc(void)
{
    _ticks++;
    if(ftick_callback!=NULL)
    {
        ftick_callback(1);
    }
    //timeout_polling();
}

UInt32 get_ticks(void)
{
    return _ticks;
}
/*
void tick_delay_us(UInt32 us)
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
*/
void tick_delay_ms(UInt32 ms)
{
    UInt32 jiffies = get_ticks();
    
    while(time_before(get_ticks(), jiffies+ms));

}

#if RTOS_EN==0
void SysTick_Handler(void)
{
    tick_inc();
}
#endif



