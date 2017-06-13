#include "iic.h"
#include "ecode.h"

#ifndef IICn
#define IICn    3
#endif


struct iic_dev *iic_devs[IICn];

e_err_t iic_register(int fd, struct iic_dev *dev)
{
    ASSERT_PARAM((fd<IICn)&&(dev!=NULL));
    
    iic_devs[fd]=dev;
    
    return E_EOK;
}

static e_size_t iic_transfer(struct iic_dev *bus,
                          struct iic_msg         msgs[],
                          e_uint32_t               num)
{
    e_size_t ret;

    if (bus->ops->master_xfer)
    {
        ret = bus->ops->master_xfer(bus, msgs, num);

        return ret;
    }
    else
    {
        return 0;
    }
}

static e_size_t iic_master_send(struct iic_dev *bus,
                             e_uint16_t               addr,
                             e_uint16_t               flags,
                             const e_uint8_t         *buf,
                             e_uint32_t               count)
{
    e_size_t ret;
    struct iic_msg msg;

    msg.addr  = addr;
    msg.flags = flags & IIC_ADDR_10BIT;
    msg.len   = count;
    msg.buf   = (e_uint8_t *)buf;

    ret = iic_transfer(bus, &msg, 1);

    return (ret > 0) ? count : ret;
}

static e_size_t iic_master_recv(struct iic_dev *bus,
                             e_uint16_t               addr,
                             e_uint16_t               flags,
                             e_uint8_t               *buf,
                             e_uint32_t               count)
{
    e_size_t ret;
    struct iic_msg msg;
    ASSERT_PARAM(bus != NULL);

    msg.addr   = addr;
    msg.flags  = flags & IIC_ADDR_10BIT;
    msg.flags |= IIC_RD;
    msg.len    = count;
    msg.buf    = buf;

    ret = iic_transfer(bus, &msg, 1);

    return (ret > 0) ? count : ret;
}

e_size_t iic_read(int fd,
                  e_uint16_t addr,
                  void *buffer,
                  e_size_t count)
{
    e_uint16_t flags;
    struct iic_dev *bus;
    
    ASSERT_PARAM(fd<IICn);
    bus = iic_devs[fd];
    ASSERT_PARAM(bus!=NULL);
    
    return iic_master_recv(bus, addr, flags, buffer, count);
}

e_size_t iic_write(int fd,
                    e_uint16_t addr,
                    const void *buffer,
                    e_size_t count)
{
    e_uint16_t flags;
    struct iic_dev *bus;
    ASSERT_PARAM(fd<IICn);
    bus = iic_devs[fd];
    ASSERT_PARAM(buffer!=NULL);
    
    return iic_master_send(bus, addr, flags, buffer, count);    
}

e_err_t iic_control(int fd,
                    e_uint8_t cmd,
                    void *args)
{
    struct iic_dev *bus;
    ASSERT_PARAM(fd<IICn);
    bus = iic_devs[fd];
    ASSERT_PARAM(bus!=NULL);
    
    switch(cmd)
    {
    case IIC_DEV_CTRL_10BIT:
        bus->flags |= IIC_ADDR_10BIT;
        break;
    case IIC_DEV_CTRL_ADDR:
        bus->addr = *(e_uint16_t *)args;
        break;
    case IIC_DEV_CTRL_TIMEOUT:
        bus->timeout = *(e_uint32_t *)args;
        break;
    case IIC_DEV_CTRL_RW:
    
        break;
    default:
        break;
    }
    
    return E_EOK;
}




