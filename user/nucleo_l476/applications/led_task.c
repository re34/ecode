#include "led_task.h"
#include "ecode.h"
#include "board.h"

static void led_task(void *args);

void led_task_init(void)
{
    rtos_task_create(led_task, "led_task", 512, NULL, 3, NULL);
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