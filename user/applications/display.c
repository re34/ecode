#include "display.h"
#include "ecode.h"
#include "led.h"
#include "includes.h"

struct timeout led_timeout;

void display_callback(void)
{
    led_toggle(LED1);
    ecode_tick_attach_timeout(&led_timeout, display_callback,500);
    
}

void display_task(void *args)
{
    LOG_DEBUG("display_task running...");
    led_init();
    ecode_tick_attach_timeout(&led_timeout, display_callback,500);
    //pwm_period_ms(PWM1,60);
    //pwm_write(PWM1, 0.5);
#if RTOS_EN==1
    while(1)
    {
        vTaskDelay(500);
    }
#endif
}


void display_init(void)
{
#if RTOS_EN==1
    LOG_DEBUG("display init...");
    xTaskCreate(display_task,
                "display_task",
                512,
                NULL,
                2,
                NULL);
#endif
}