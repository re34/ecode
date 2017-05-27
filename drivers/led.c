#include "led.h"



void led_init(void)
{
    bsp_pin_init(LED1, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
}

