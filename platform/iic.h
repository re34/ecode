#ifndef __IIC_H__
#define __IIC_H__
#include "sim_iic.h"

enum{
    IIC1,
    IIC2,
    IICn
};

enum{
    IIC_ACK,
    IIC_NACK,
};

enum{
    IIC_TYPE_SIM,
    IIC_TYPE_MCU,
};

struct iic_dev{
    int iic_type;
    void *ops;
};

int iic_register(int fd, struct iic_dev *dev);
int iic_bus_start(int fd);
int iic_bus_stop(int fd);
int iic_bus_write(int fd, int byte, int ack);
int iic_bus_read(int fd);
int iic_bus_read_ack(int fd);

#endif