#ifndef __HWTIMER_H__
#define __HWTIMER_H__


struct hwtimer_operations{
    void (*start)(struct hwtimer_dev *dev);
    void (*stop)(struct hwtimer_dev *dev);
    e_uint32_t (*read)(struct hwtimer_dev *dev);
    e_uint32_t (*write)(struct hwtimer_dev *dev);
};

struct hwtimer_dev{
    e_uint32_t freq;
    e_uint32_t reload;
    e_uint32_t cycles;
    const struct hwtimer_operations *ops;
    void *callback;
};


#endif