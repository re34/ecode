#ifndef __SIM_IIC_H__
#define __SIM_IIC_H__
#include "includes.h"


struct sim_iic_operations{
    int (*get_sda)(void);
    void (*scl_set)(void);
    void (*scl_reset)(void);
    void (*sda_set)(void);
    void (*sda_reset)(void);
};


int sim_iic_start(struct sim_iic_operations *dev);
int sim_iic_stop(struct sim_iic_operations *dev);
int sim_iic_write(struct sim_iic_operations *dev, UInt8 data, UInt8 ack);
int sim_iic_read(struct sim_iic_operations *ops);
int sim_iic_read_ack(struct sim_iic_operations *dev);
//int sim_iic_ack(struct sim_iic_operations *dev);
//int sim_iic_nack(struct sim_iic_operations *dev);

#endif