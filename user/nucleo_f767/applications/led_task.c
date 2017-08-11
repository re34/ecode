#include "led_task.h"
#include "ecode.h"
#include "board.h"

static void led_task(void *args);

void led_task_init(void)
{
    os_thread_t led_thread={
        .name = "led",
        .pthread = led_task,
        .stacksz = 512,
        .priority = OS_PRIO_NORMAL,
        .args = NULL,
    };

    os_thread_new(&led_thread);
}

static void led_task(void *args)
{
    pin_mode(LED1, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    
    for(;;)
    {
        pin_write(LED1,!pin_read(LED1));
        delay_ms(500);
    }
}