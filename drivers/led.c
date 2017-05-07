#include "led.h"



void led_init(void)
{
    bsp_pin_digital_out_init(LED1, PIN_PULLUP);
}

