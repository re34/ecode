#ifndef __IIC_H__
#define __IIC_H__
#include "ecode.h"

#define IIC_DEV_CTRL_10BIT      0x20
#define IIC_DEV_CTRL_ADDR       0x21
#define IIC_DEV_CTRL_TIMEOUT    0x22
#define IIC_DEV_CTRL_RW         0x23

#define IIC_WR                0x0000
#define IIC_RD               (1u << 0)
#define IIC_ADDR_10BIT       (1u << 2)  /* this is a ten bit chip address */
#define IIC_NO_START         (1u << 4)
#define IIC_IGNORE_NACK      (1u << 5)
#define IIC_NO_READ_ACK      (1u << 6)  /* when IIC reading, we do not ACK */


struct iic_msg{
    e_uint16_t addr;
    e_uint16_t flags;
    e_uint16_t len;
    e_uint8_t *buf;
};

struct iic_dev{
    const struct iic_operations *ops;
    e_uint16_t flags;
    e_uint16_t addr;
    e_uint32_t timeout;
    e_uint32_t retries;
    void *priv;
};

struct iic_operations{
    e_size_t (*master_xfer)(struct iic_dev *bus,
                            struct iic_msg msgs[],
                            e_uint32_t num);
    e_size_t (*slave_xfer)(struct iic_dev *bus,
                            struct iic_msg msgs[],
                            e_uint32_t num);
    e_err_t (*iic_control)(struct iic_dev *bus,
                            e_uint32_t,
                            e_uint32_t);
};


e_err_t iic_register(int fd, struct iic_dev *dev);
e_size_t iic_read(int fd,
                  e_uint16_t addr,
                  void *buffer,
                  e_size_t count);
e_size_t iic_write(int fd,
                    e_uint16_t addr,
                    const void *buffer,
                    e_size_t count);
e_err_t iic_control(int fd,
                    e_uint8_t cmd,
                    void *args);
#endif