#include "time.h"
#include "rtos.h"

static struct time_operations _ops;

static e_uint32_t _system_time = 0;

e_err_t time_init(struct time_operations ops)
{
    _ops = ops;
    
    return E_EOK;
}

void system_time_inc(void)
{
    _system_time++;
}

e_uint32_t system_get_time(void)
{
    return _system_time;
}

void bare_delay_ms(e_uint32_t ms)
{
    e_uint32_t jiffies = _system_time;
    
    while(time_before(_system_time, jiffies+ms));
}

void bare_delay_us(e_uint32_t us)
{
    if(_ops.udelay!=NULL)
        _ops.udelay(us);
}

void delay_ms(e_uint32_t ms)
{
#if RTOS_EN==1
    TickType_t ticks = ms/portTICK_PERIOD_MS;
#if (INCLUDE_xTaskGetSchedulerState == 1)
    if(xTaskGetSchedulerState()==taskSCHEDULER_NOT_STARTED)
    {
        bare_delay_ms(ms);
    }
    else
    {
        vTaskDelay(ticks?ticks:1);
    }
#else
    vTaskDelay(ticks?ticks:1);
#endif	
#else
	bare_delay_ms(ms);
#endif
}

void delay_us(e_uint32_t us)
{
    if(_ops.udelay!=NULL)
        _ops.udelay(us);
}