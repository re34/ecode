#include "display.h"
#include "ecode.h"
#include "led.h"
#include "includes.h"
#include "ecode.h"

struct timeout led_timeout;


void display_callback(void)
{
    led_toggle(LED1);
    tick_attach_timeout(&led_timeout, display_callback,500);
    
}

void display_task(void *args)
{
    LOG_DEBUG("display_task running...");
    led_init();
    wait_system_on();
    tick_attach_timeout(&led_timeout, display_callback,500);
    //pwm_period_ms(PWM1,500);
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
    xTaskCreate(display_task,
                "display_task",
                1024,
                NULL,
                3,
                NULL);
#endif
}



