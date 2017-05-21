#include "pwm.h"
#include "target.h"
#include "ecode.h"

static struct pwm_device *pwm_devs[PWMn];

int pwm_register(pwm_name_t pwm, struct pwm_device *dev)
{
    struct pwm_operations *ops;
    
    if(pwm>=PWMn||dev==NULL)
        return -1;
    
    ops = dev->ops;
    
    if(ops==NULL)
        return -2;
    
    if(ops->init==NULL||ops->pulsewidth_us==NULL||ops->period_us==NULL)
        return -3;
    
    if(ops->init()<0)
    {
        return -4;
    }
    
    pwm_devs[pwm]= dev;
    
    LOG_DEBUG("PWM%d is registered!", (int)pwm);
    return 0;
}



int pwm_write(pwm_name_t pwm, float duty)
{
    struct pwm_device *dev;
    int us;
   
    dev = pwm_devs[pwm];
    
    if(dev==NULL)
        return -1;
    us = (int)(dev->period_us*duty);
    
    dev->ops->pulsewidth_us(us);
    
    dev->pulsewidth_us = us;
    
    return 0;
}

float pwm_read(pwm_name_t pwm)
{
    struct pwm_device *dev;
    
    dev = pwm_devs[pwm];
    
    if(dev==NULL)
        return -1;
    
    return dev->pulsewidth_us/dev->period_us;
}

int pwm_period_ms(pwm_name_t pwm, int ms)
{
    return pwm_period_us(pwm, ms*1000);
}

int pwm_period_us(pwm_name_t pwm, int us)
{
    struct pwm_device *dev;
    
    if(pwm_devs[pwm]==NULL)
        return -1;
    
    dev = pwm_devs[pwm];
    
    dev->period_us = us;
    
    dev->ops->period_us(dev->period_us);    
    
    return 0;
}

int pwm_pulsewidth(pwm_name_t pwm, float seconds)
{
    return pwm_pulsewidth_us(pwm, (int)(seconds*1000000));
}

int pwm_pulsewidth_ms(pwm_name_t pwm, int ms)
{
    return pwm_pulsewidth_us(pwm, ms*1000);
}

int pwm_pulsewidth_us(pwm_name_t pwm, int us)
{
    struct pwm_device *dev;
    
    dev = pwm_devs[pwm];
    
    if(dev==NULL)
        return -1;
    
    if(us>dev->period_us)
        us = dev->period_us;
    
    dev->pulsewidth_us = us;
    
    dev->ops->pulsewidth_us(us);

    return 0;
}




