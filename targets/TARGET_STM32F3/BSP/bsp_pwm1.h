#ifndef __BSP_PWM1_H__
#define __BSP_PWM1_H__


int bsp_pwm1_init(void);
void bsp_pwm1_period_us(int us);
void bsp_pwm1_pulsewidth_us(int us);
#endif