#include "iic.h"
#include "ecode.h"

#ifndef IICn
#define IICn    3
#endif


struct iic_dev *iic_devs[IICn];

e_err_t iic_register(int fd, struct iic_dev *dev)
{
    struct iic_operations *ops;
    ASSERT_PARAM((fd<IICn)&&(dev!=NULL));
    
    iic_devs[fd]=dev;
    
    return E_EOK;
}

e_err_t iic_bus_start(int fd)
{
    struct iic_dev *dev;
    struct iic_operations *ops;

    ASSERT_PARAM(fd<IICn);
    dev = iic_devs[fd];
    ASSERT_PARAM(dev!=NULL);
    ops = dev->ops;
    ASSERT_PARAM(ops!=NULL);
    
    return ops->start(dev);
}

void iic_bus_stop(int fd)
{
    struct iic_dev *dev;
    struct iic_operations *ops;

    ASSERT_PARAM(fd<IICn);
    dev = iic_devs[fd];
    ASSERT_PARAM(dev!=NULL);
    ops = dev->ops;
    ASSERT_PARAM(ops!=NULL);
    
    ops->stop(dev);
}

e_err_t iic_bus_write(int fd, e_uint8_t byte, e_uint8_t *ack)
{
    struct iic_dev *dev;
    struct iic_operations *ops;

    ASSERT_PARAM(fd<IICn);
    dev = iic_devs[fd];
    ASSERT_PARAM(dev!=NULL);
    ops = dev->ops;
    ASSERT_PARAM(ops!=NULL);
    
    return ops->write(dev, byte, ack);
}


e_uint8_t iic_bus_read(int fd, e_uint8_t ack)
{
    struct iic_dev *dev;
    struct iic_operations *ops;

    ASSERT_PARAM(fd<IICn);
    dev = iic_devs[fd];
    ASSERT_PARAM(dev!=NULL);
    ops = dev->ops;
    ASSERT_PARAM(ops!=NULL);
    
    return ops->read(dev, ack);
}




