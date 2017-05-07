#ifndef __LED_H__
#define __LED_H__
#include "target.h"

enum{
    LED1=PA_5,
};

#define LED1    PA_5


#define led_on(led)     bsp_pin_set(led)
#define led_off(led)    bsp_pin_off(led)
#define led_toggle(led)     bsp_pin_toggle(led)

void led_init(void);

#endif