#ifndef __PWM_H__
#define __PWM_H__


typedef enum{
    PWM1,
    PWM2,
    PWM3,
    PWMn,
}pwm_name_t;

struct pwm_operations{
    int (*init)(void);
    void (*pulsewidth_us)(int us);
    void (*period_us)(int us);
};

struct pwm_device{
    int period_us;
    int pulsewidth_us;
    struct pwm_operations *ops;
};

int pwm_register(pwm_name_t pwm, struct pwm_device *dev);
int pwm_write(pwm_name_t pwm, float duty);
float pwm_read(pwm_name_t pwm);
int pwm_period_ms(pwm_name_t pwm, int ms);
int pwm_period_us(pwm_name_t pwm, int us);
int pwm_pulsewidth(pwm_name_t pwm, float seconds);
int pwm_pulsewidth_ms(pwm_name_t pwm, int ms);
int pwm_pulsewidth_us(pwm_name_t pwm, int us);

#endif