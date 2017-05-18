#ifndef __bsp_base_timer22_H__
#define __bsp_base_timer22_H__
#include "target_chip.h"


int bsp_base_timer2_init(void);
void bsp_base_timer2_start(void);
void bsp_base_timer2_stop(void);
void bsp_base_timer2_reset(void);
float bsp_base_timer2_read(void);
int bsp_base_timer2_read_ms(void);
int bsp_base_timer2_read_us(void);


#endif