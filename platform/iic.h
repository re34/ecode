#ifndef __IIC_H__
#define __IIC_H__
#include "sim_iic.h"

enum{
    IIC_ACK,
    IIC_NACK,
};

enum{
    IIC_TYPE_SIM,
    IIC_TYPE_MCU,
};

struct iic_operations{
    e_err_t (*start)(struct iic_dev *dev);
    void (*stop)(struct iic_dev *dev);
    e_err_t (*write)(struct iic_dev *dev, e_uint8_t byte, e_uint8_t *ack);
    e_err_t (*read)(struct iic_dev *dev, e_uint8_t ack);
};

struct iic_dev{
    struct iic_operations *ops;
    void *private_data;
};

e_err_t iic_register(int fd, struct iic_dev *dev);
e_err_t iic_bus_start(int fd);
void iic_bus_stop(int fd);
e_err_t iic_bus_write(int fd, e_uint8_t byte, e_uint8_t *ack);
e_uint8_t iic_bus_read(int fd, e_uint8_t ack);
#endif