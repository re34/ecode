#ifndef __IIC_H__
#define __IIC_H__
#include "types.h"


struct iic_dev{
    const struct iic_operations *ops;
};

struct iic_operations{
    e_err_t (*start)(struct iic_dev *bus);
    e_err_t (*stop)(struct iic_dev *bus);
    int (*writeb)(struct iic_dev *bus , e_uint8_t value);
    e_err_t (*readb)(struct iic_dev *bus, e_uint8_t *pvalue, int ack);
};

e_err_t iic_start(int bus);
e_err_t iic_stop(int bus);
int iic_writeb(int bus, e_uint8_t value);
e_err_t iic_readb(int bus, e_uint8_t *pvalue, int ack);
e_err_t iic_register(int bus, struct iic_dev *iic_dev);
#endif