#ifndef __SIM_IIC_H__
#define __SIM_IIC_H__
#include "includes.h"


struct sim_iic_operations{
    int (*get_sda)(void);
    void (*scl_set)(void);
    void (*scl_reset)(void);
    void (*sda_set)(void);
    void (*sda_reset)(void);
    void (*delay)(int delay);
};

struct sim_iic_dev{
    struct iic_dev parent;
    int delay;
    const struct sim_iic_operations *ops;
};


e_err_t sim_iic_register(int fd, struct sim_iic_dev *dev);


int sim_iic_start(struct sim_iic_operations *dev);
int sim_iic_stop(struct sim_iic_operations *dev);
int sim_iic_write(struct sim_iic_operations *dev, UInt8 data, UInt8 ack);
int sim_iic_read(struct sim_iic_operations *ops);
#endif