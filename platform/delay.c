#include "delay.h"
#include "ecode.h"


void delay_ms(unsigned int ms)
{
#if RTOS_EN==1
    TickType_t ticks = ms/portTICK_PERIOD_MS;
#if (INCLUDE_xTaskGetSchedulerState == 1)
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)
    {
        tick_delay_ms(ms);
    }
    else
    {
        vTaskDelay(ticks?ticks:1);
    }
#else
    vTaskDelay(ticks?ticks:1);
#endif	
#else
	tick_delay_ms(ms);
#endif
}

void delay_us(int us)
{
	//tick_delay(us);
}