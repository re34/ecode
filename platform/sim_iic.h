#ifndef __SIM_IIC_H__
#define __SIM_IIC_H__
#include "includes.h"


struct sim_iic_operations{
    void *data;             //private data for lowlevel
    void (*set_sda)(void *data, e_int32_t state);
    void (*set_scl)(void *data, e_int32_t state);
    e_int32_t (*get_sda)(void *data);
    e_int32_t (*get_scl)(void *data);
    e_uint32_t delay_us;
    e_uint32_t timeout;
    void (*udelay)(e_uint32_t us);
};

struct sim_iic_dev{
    struct iic_dev parent;
    const struct sim_iic_operations *ops;
};


e_err_t sim_iic_register(int fd, struct sim_iic_dev *dev);


int sim_iic_start(struct sim_iic_operations *dev);
int sim_iic_stop(struct sim_iic_operations *dev);
int sim_iic_writeb(struct sim_iic_operations *dev, UInt8 data, UInt8 ack);
int sim_iic_readb(struct sim_iic_operations *ops);
#endif