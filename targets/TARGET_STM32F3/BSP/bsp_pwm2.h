#ifndef __BSP_PWM2_H__
#define __BSP_PWM2_H__


int bsp_pwm2_init(void);
void bsp_pwm2_period_us(int us);
void bsp_pwm2_pulsewidth_us(int us);
#endif