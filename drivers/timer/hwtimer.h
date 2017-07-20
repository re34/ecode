#ifndef __HWTIMER_H__
#define __HWTIMER_H__
#include "ecode.h"

#define HWTIMER_EVENT_UPDATE        (1<<0)
#define HWTIMER_EVENT_CAPTURE        (1<<1)

struct hwtimer{
    e_uint32_t period;
    e_uint32_t period_cnt;
    e_uint32_t freq;
    void *private_data;
    const struct hwtimer_operations *ops;
};

struct hwtimer_operations{
    long (*read_cnt)(struct hwtimer *hwtimer);
    void (*start)(struct hwtimer *hwtimer);
    void (*stop)(struct hwtimer *hwtimer);
    void (*reset)(struct hwtimer *hwtimer);
    void (*isr_update)(struct hwtimer *hwtimer);
    void (*isr_capture)(struct hwtimer *hwtimer);
};



e_err_t hwtimer_register(int fd, struct hwtimer *hwtimer);
e_err_t hwtimer_start(int fd);
e_err_t hwtimer_stop(int fd);
e_err_t hwtimer_reset(int fd);
long hwtimer_read_us(int fd);
long hwtimer_read_ms(int fd);
float hwtimer_read_sec(int fd);
void hwtimer_isr(struct hwtimer *hwtimer,int event);

#endif