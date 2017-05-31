#include "delay.h"
#include "ecode.h"


void delay_ms(int ms)
{
#if RTOS_EN==1
	vTaskDelay(ms);
#else
	tick_delay_ms(ms);
#endif
}

void delay_us(int us)
{
	//tick_delay(us);
}