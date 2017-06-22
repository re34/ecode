#include "hwtimer.h"

#ifndef HWTIMERn
#define HWTIMERn        3
#endif

struct hwtimer_dev hwtimers[HWTIMERn];



e_uint32_t hwtimer_read(int fd)
{
    
    
    return 0;
}

e_err_t hwtimer_write(int fd, e_uint32_t data)
{
    
    
    
    return E_EOK;
}


e_err_t hwtimer_register(int fd, struct hwtimer_dev *dev)
{
    ASSERT_PARAM(dev!=NULL);
    ASSERT_PARAM(dev->ops!=NULL);
    
    return E_EOK;
}