#ifndef __BSP_GPIO_H__
#define __BSP_GPIO_H__
#include "target_chip.h"


extern const uint32_t pin_defines[16];

#define STM_PIN(pin)    ((pin)&0xF)  
#define STM_PORT(pin)   (((pin)>>4)&0x0F)



typedef enum{
	PIN_MODE_INPUT,
	PIN_MODE_OUTPUT_PP,
	PIN_MODE_OUTPUT_OD,
	PIN_MODE_ANALOG,
}pin_mode_t;

typedef enum{
	PIN_PUPD_NO,
	PIN_PUPD_UP,
	PIN_PUPD_DOWN,
}pin_pupd_t;

typedef enum{
	PIN_RESET=0,
	PIN_SET	= !PIN_RESET,
}pin_state_t;

GPIO_TypeDef *bsp_gpio_set_clk(uint32_t port_idx);
void bsp_pin_init(pin_name_t pin, pin_mode_t mode, pin_pupd_t pull);
void bsp_pin_set(pin_name_t pin);
void bsp_pin_reset(pin_name_t pin);
pin_state_t bsp_pin_read(pin_name_t pin);
void bsp_pin_toggle(pin_name_t pin);
#endif