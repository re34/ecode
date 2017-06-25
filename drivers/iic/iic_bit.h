#ifndef __IIC_BIT_H__
#define __IIC_BIT_H__
#include "iic.h"

struct iic_bit_operations{
    void (*set_scl)(void *data, int bit);
    void (*set_sda)(void *data, int bit);
    int (*get_sda)(void *data);
    int (*get_scl)(void *data);
    void (*udelay)(int us);
    int timeout;
    int delay_us;
    void *data;
};

struct iic_bit_dev{
    struct iic_dev parent;
    const struct iic_bit_operations *ops;
};


e_err_t iic_bit_register(int bus,struct iic_bit_dev *iic_bit_dev);
#endif