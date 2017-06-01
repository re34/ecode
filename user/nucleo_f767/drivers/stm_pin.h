#ifndef __STM_GPIO_H__
#define __STM_GPIO_H__
#include "stm32f7xx_hal.h"
#include "pin_names.h"
#include "port_names.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"

#include "stm32f7xx_ll_gpio.h"

#include "stm32f7xx_ll_usart.h"

#include "stm32f7xx_ll_tim.h"

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

GPIO_TypeDef *stm_gpio_set_clk(uint32_t port_idx);
void stm_pin_init(pin_name_t pin, pin_mode_t mode, pin_pupd_t pull);
void stm_pin_set(pin_name_t pin);
void stm_pin_reset(pin_name_t pin);
pin_state_t stm_pin_read(pin_name_t pin);
void stm_pin_toggle(pin_name_t pin);
#endif