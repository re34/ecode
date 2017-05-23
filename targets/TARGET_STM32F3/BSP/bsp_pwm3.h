#ifndef __BSP_PWM3_H__
#define __BSP_PWM3_H__


int bsp_pwm3_init(void);
void bsp_pwm3_period_us(int us);
void bsp_pwm3_pulsewidth_us(int us);
#endif