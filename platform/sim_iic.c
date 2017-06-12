#include "sim_iic.h"
#include "iic.h"


void sim_iic_delay(int delay)
{
    
}

e_err_t sim_iic_start(struct iic_dev *dev)
{
    int timeout = 0xff;
    struct sim_iic_dev *sim_iic;
    struct sim_iic_operations *ops;
    
    ASSERT_PARAM(dev!=NULL);
    sim_iic = dev->private_data;
    ASSERT_PARAM(sim_iic!=NULL);
    ops = sim_iic->ops;
    ASSERT_PARAM(ops!=NULL);
    
    ops->sda_set();
    ops->scl_set();
    ops->delay(sim_dev->delay);
    
    while(ops->get_sda()==0)
    {
        timeout--;
        if(timeout==0)
            return -E_ERROR;
    }
    ops->sda_reset();
    ops->delay(sim_dev->delay);
    ops->sda_reset();
    ops->delay(sim_dev->delay);
    
    return E_EOK;
}

void sim_iic_stop(struct iic_dev *dev)
{
    struct sim_iic_dev *sim_iic;
    struct sim_iic_operations *ops;
    
    ASSERT_PARAM(dev!=NULL);
    sim_iic = dev->private_data;
    ASSERT_PARAM(sim_iic!=NULL);
    ops = sim_iic->ops;
    ASSERT_PARAM(ops!=NULL);
    
    ops->sda_reset();
    ops->delay(sim_dev->delay);
    ops->scl_set();
    ops->delay(sim_dev->delay);
    ops->sda_set();
    ops->delay(sim_dev->delay);
}

e_err_t sim_iic_write(struct iic_dev *dev, e_uint8_t data, e_uint8_t *ack)
{
    int i;
    struct sim_iic_dev *sim_iic;
    struct sim_iic_operations *ops;
    
    ASSERT_PARAM(dev!=NULL);
    sim_iic = dev->private_data;
    ASSERT_PARAM(sim_iic!=NULL);
    ops = sim_iic->ops;
    ASSERT_PARAM(ops!=NULL);
    
    for(i=0;i<8;i++)
    {
        ops->scl_reset();
        if(data&0x80)
            ops->sda_set();
        else
            ops->sda_reset();
        ops->delay(sim_dev->delay);
        ops->scl_set();
        ops->delay(sim_dev->delay);
        ops->scl_reset();
        ops->delay(sim_dev->delay);
        data<<=1;
    }
    
    if(ack==IIC_ACK)
    {
        ops->sda_reset();
        ops->delay(sim_dev->delay);
        ops->scl_set();
        ops->delay(sim_dev->delay);
        ops->scl_reset();
        ops->delay(sim_dev->delay);
    }
    else
    {
        ops->sda_set();
        ops->delay(sim_dev->delay);
        ops->scl_set();
        ops->delay(sim_dev->delay);
        ops->scl_reset();
        ops->delay(sim_dev->delay);
    }
    
    return 0;
}

e_uint8_t sim_iic_read(struct sim_iic_operations *ops, e_uint8_t ack)
{
    int data = 0;
    int i;
    
    ops->sda_set();
    ops->scl_reset();
    ops->delay(sim_dev->delay);
    for(i=0;i<8;i++)
    {
        data<<=1;
        ops->scl_set();
        ops->delay(sim_dev->delay);
        if(ops->get_sda()==1)
            data|=0x01;
        ops->scl_reset();
        ops->delay(sim_dev->delay);
        
    }
        
    return data;
}



e_err_t sim_iic_register(int fd, struct sim_iic_dev *dev)
{
    e_err_t err;
    
    
    
    err = iic_register(fd, dev);
    
    
    
    
    return E_EOK;
}

