#ifndef __BSP_PWM_H__
#define __BSP_PWM_H__


int bsp_pwm_init(void);
void bsp_pwm_period_us(int us);
void bsp_pwm_pulsewidth_us(int us);
#endif