#include "timer.h"
#include "includes.h"


struct timer_device *timer_devs[TIMERn]={0};


int timer_register(int fd, struct timer_device *dev)
{
    if(fd>TIMERn||dev==NULL)
        return -1;
    
    if(dev->init==NULL)
        return -2;
    
    if(dev->init()<0)
        return -3;
    
    if((dev->start==NULL)||(dev->stop==NULL)||(dev->reset==NULL)\
        ||(dev->read==NULL)||(dev->read_ms==NULL)||(dev->read_us==NULL))
        return -4;
    
    timer_devs[fd]=dev;
    
    LOG_DEBUG("TIMER%d is registered!", fd+1);
    return 0;
}

int timer_start(int fd)
{
    struct timer_device *dev;
    
    if(fd>=TIMERn)
        return -1;
    
    dev = timer_devs[fd];
    
    if(dev==NULL)
        return -2;
    dev->start();
    
    return 0;
}


int timer_stop(int fd)
{
    struct timer_device *dev;
    
    if(fd>=TIMERn)
        return -1;
    
    dev = timer_devs[fd];
    
    if(dev==NULL)
        return -2;
    
    dev->stop();
    
    return 0;
}

int timer_reset(int fd)
{
    struct timer_device *dev;
    
    if(fd>=TIMERn)
        return -1;
    
    dev = timer_devs[fd];
    
    if(dev==NULL)
        return -2;
    
    dev->reset();
    
    return 0;
}

float timer_read(int fd)
{
    struct timer_device *dev;
    
    if(fd>=TIMERn)
        return -1;
    
    dev = timer_devs[fd];
    
    if(dev==NULL)
        return -2;
    
    return dev->read();
}

int timer_read_ms(int fd)
{
    struct timer_device *dev;
    
    if(fd>=TIMERn)
        return -1;
    
    dev = timer_devs[fd];
    
    if(dev==NULL)
        return -2;
    
    return dev->read_ms();
}

int timer_read_us(int fd)
{
    struct timer_device *dev;
    
    if(fd>=TIMERn)
        return -1;
    
    dev = timer_devs[fd];
    
    if(dev==NULL)
        return -2;
    
    return dev->read_us();
}



