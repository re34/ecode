#ifndef __PWM_H__
#define __PWM_H__

struct pwm_operations{
    int (*init)(void);
    void (*period)(unsigned int hz);
    void (*pulseduty)(float duty);
    void (*pulsewidth_us)(int us);
    void (*period_us)(int us);
};

struct pwm_device{
    int period_us;
    int pulsewidth_us;
    struct pwm_operations *ops;
};

int pwm_register(int pwm, struct pwm_device *dev);
int pwm_write(int pwm, float duty);
float pwm_read(int pwm);
int pwm_period(int pwm, int hz);
int pwm_period_ms(int pwm, int ms);
int pwm_period_us(int pwm, int us);
int pwm_pulsewidth(int pwm, float seconds);
int pwm_pulsewidth_ms(int pwm, int ms);
int pwm_pulsewidth_us(int pwm, int us);

#endif