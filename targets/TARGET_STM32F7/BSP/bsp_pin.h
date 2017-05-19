#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__
#include "target_chip.h"


extern const uint32_t pin_defines[16];

#define STM_PIN(pin)    ((pin)&0xF)  
#define STM_PORT(pin)   (((pin)>>4)&0x0F)



enum{
  PIN_PULLUP,
  PIN_PULLPD,
};

GPIO_TypeDef *bsp_gpio_set_clk(uint32_t port_idx);
void bsp_pin_digital_out_init(pin_name_t pin, uint8_t pull);
void bsp_pin_set(pin_name_t pin);
void bsp_pin_reset(pin_name_t pin);
void bsp_pin_toggle(pin_name_t pin);
#endif