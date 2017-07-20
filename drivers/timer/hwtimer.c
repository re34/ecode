#include "hwtimer.h"


#ifndef HWTIMERn
#define HWTIMERn    3
#endif

static struct hwtimer * fhwtimer[HWTIMERn];

e_err_t hwtimer_register(int fd, struct hwtimer *hwtimer)
{
    ASSERT_PARAM(hwtimer!=NULL);
    ASSERT_PARAM(hwtimer->ops!=NULL);
    
    if(fd<0||fd>=HWTIMERn)
        return E_ERROR;
    
    fhwtimer[fd]=hwtimer;
    
    return E_EOK;
}

e_err_t hwtimer_start(int fd)
{
    struct hwtimer *hwtimer;
    const struct hwtimer_operations *ops;
    
    if(fd<0||fd>=HWTIMERn)
        return E_ERROR;
    hwtimer = fhwtimer[fd];
    ASSERT_PARAM(hwtimer!=NULL);
    ops = hwtimer->ops;
    
    ASSERT_PARAM(ops!=NULL);
    
    if(ops->start==NULL)
        return E_ERROR;
    
    ops->start(hwtimer);
    hwtimer->period_cnt = 0;
    
    return E_EOK;
}

e_err_t hwtimer_stop(int fd)
{
    struct hwtimer *hwtimer;
    const struct hwtimer_operations *ops;
    
    if(fd<0||fd>=HWTIMERn)
        return E_ERROR;
    hwtimer = fhwtimer[fd];
    ASSERT_PARAM(hwtimer!=NULL);
    ops = hwtimer->ops;
    
    ASSERT_PARAM(ops!=NULL);
    
    if(ops->stop==NULL)
        return E_ERROR;
    
    ops->stop(hwtimer);
    
    return E_EOK;
}

e_err_t hwtimer_reset(int fd)
{
    struct hwtimer *hwtimer;
    const struct hwtimer_operations *ops;
    
    if(fd<0||fd>=HWTIMERn)
        return E_ERROR;
    hwtimer = fhwtimer[fd];
    ASSERT_PARAM(hwtimer!=NULL);
    ops = hwtimer->ops;
    
    ASSERT_PARAM(ops!=NULL);
    
    if(ops->reset==NULL)
        return E_ERROR;
    
    ops->reset(hwtimer);
    hwtimer->period_cnt = 0;
    
    
    return E_EOK;
}

long hwtimer_read_us(int fd)
{
    struct hwtimer *hwtimer;
    const struct hwtimer_operations *ops;
    long cnt;
    
    if(fd<0||fd>=HWTIMERn)
        return 0;
    hwtimer = fhwtimer[fd];
    ASSERT_PARAM(hwtimer!=NULL);
    ops = hwtimer->ops;
    
    ASSERT_PARAM(ops!=NULL);
    
    if(ops->read_cnt==NULL)
        return 0;
    
    cnt = ops->read_cnt(hwtimer);
    
    return (cnt+hwtimer->period*hwtimer->period_cnt)*1000000/hwtimer->freq;
}

long hwtimer_read_ms(int fd)
{
    struct hwtimer *hwtimer;
    const struct hwtimer_operations *ops;
    long cnt;
    
    if(fd<0||fd>=HWTIMERn)
        return 0;
    hwtimer = fhwtimer[fd];
    ASSERT_PARAM(hwtimer!=NULL);
    ops = hwtimer->ops;
    
    ASSERT_PARAM(ops!=NULL);
    
    if(ops->read_cnt==NULL)
        return 0;
    
    cnt = ops->read_cnt(hwtimer);
    
    return (cnt+hwtimer->period*hwtimer->period_cnt)*1000/hwtimer->freq;
}

float hwtimer_read_sec(int fd)
{
    struct hwtimer *hwtimer;
    const struct hwtimer_operations *ops;
    long cnt;
    
    if(fd<0||fd>=HWTIMERn)
        return 0;
    hwtimer = fhwtimer[fd];
    ASSERT_PARAM(hwtimer!=NULL);
    ops = hwtimer->ops;
    
    ASSERT_PARAM(ops!=NULL);
    
    if(ops->read_cnt==NULL)
        return 0;
    
    cnt = ops->read_cnt(hwtimer);
    
    return (cnt+hwtimer->period*hwtimer->period_cnt)*1.0f/hwtimer->freq;
}


void hwtimer_isr(struct hwtimer *hwtimer,int event)
{
    switch(event)
    {
    case HWTIMER_EVENT_UPDATE:
        hwtimer->period_cnt++;
        if(hwtimer->ops->isr_update!=NULL)
            hwtimer->ops->isr_update(hwtimer);
        break;
    case HWTIMER_EVENT_CAPTURE:
        if(hwtimer->ops->isr_capture!=NULL)
            hwtimer->ops->isr_capture(hwtimer);
        break;
    }
}


