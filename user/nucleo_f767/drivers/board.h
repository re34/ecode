#ifndef __BOARD_H__
#define __BOARD_H__
#include "ecode.h"

#include "stm32f7xx_hal.h"
#include "stm32f7xx_ll_bus.h"
#include "stm32f7xx_ll_rcc.h"

#include "stm32f7xx_ll_gpio.h"

#include "stm32f7xx_ll_usart.h"

#include "stm32f7xx_ll_tim.h"

#include "stm_pin.h"

#include "lan8720.h"

void board_clock_configuration(void);
void ecode_hw_board_init();
#endif