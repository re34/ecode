#ifndef __BSP_BASE_TIMER_H__
#define __BSP_BASE_TIMER_H__
#include "target_chip.h"


int bsp_base_timer_init(void);
void bsp_base_timer_start(void);
void bsp_base_timer_stop(void);
void bsp_base_timer_reset(void);
float bsp_base_timer_read(void);
int bsp_base_timer_read_ms(void);
int bsp_base_timer_read_us(void);


#endif