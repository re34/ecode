#include "iic.h"
#include "assert.h"


#ifndef IICn
#define IICn    2
#endif

static struct iic_dev *_iics[IICn];


e_err_t iic_start(int bus)
{
    struct iic_dev *iic_bus = _iics[bus];
    ASSERT_PARAM(iic_bus!=NULL);
    const struct iic_operations *ops = iic_bus->ops;
    ASSERT_PARAM(ops!=NULL);
    
    if(ops->start==NULL)
        return -E_EOK;
    
    return ops->start(iic_bus);
}
e_err_t iic_stop(int bus)
{
    struct iic_dev *iic_bus = _iics[bus];
    ASSERT_PARAM(iic_bus!=NULL);
    const struct iic_operations *ops = iic_bus->ops;
    ASSERT_PARAM(ops!=NULL);
    
    if(ops->stop==NULL)
        return -E_EOK;
    
    return ops->stop(iic_bus);
}
int iic_writeb(int bus, e_uint8_t value)
{
    struct iic_dev *iic_bus = _iics[bus];
    ASSERT_PARAM(iic_bus!=NULL);
    const struct iic_operations *ops = iic_bus->ops;
    ASSERT_PARAM(ops!=NULL);
    
    if(ops->writeb==NULL)
        return -E_EOK;
    
    return ops->writeb(iic_bus, value);
}
e_err_t iic_readb(int bus, e_uint8_t *pvalue , int ack)
{
    struct iic_dev *iic_bus = _iics[bus];
    ASSERT_PARAM(iic_bus!=NULL);
    const struct iic_operations *ops = iic_bus->ops;
    ASSERT_PARAM(ops!=NULL);
    
    if(ops->readb==NULL)
        return -E_EOK;
    
    return ops->readb(iic_bus, pvalue, ack);
}

e_err_t iic_register(int bus, struct iic_dev *iic_dev)
{
    ASSERT_PARAM(iic_dev!=NULL);
    ASSERT_PARAM(bus<IICn);
    
    _iics[bus]=iic_dev;
    
    return E_EOK;
}