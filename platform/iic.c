#include "iic.h"
#include "ecode.h"

struct iic_dev *iic_devs[IICn];

int iic_register(int fd, struct iic_dev *dev)
{
    if((fd>=IICn)||(dev->ops==NULL))
        return -1;
    
    iic_devs[fd]=dev;
    
    return 0;
}

int iic_bus_start(int fd)
{
    struct iic_dev *dev = iic_devs[fd];
    int ret = 0;
    
    if(dev==NULL)
        return -1;
    
    if(dev->iic_type==IIC_TYPE_SIM)
    {
        if(iic_sim_start(dev->ops)<0)
            return -2;
    }
    else
    {
        
    }
    
    return 0;
}

int iic_bus_stop(int fd)
{
    struct iic_dev *dev = iic_devs[fd];
    
    if(dev==NULL)
        return -1;
    
    if(dev->iic_type==IIC_TYPE_SIM)
    {
        if(iic_sim_stop(dev->ops)<0)
            return -2;
    }
    else
    {
        
    }
    
    return 0;
}


int iic_bus_write(int fd, int byte, int ack)
{
    struct iic_dev *dev = iic_devs[fd];
    
    if(dev==NULL)
        return -1;
    
    if(dev->iic_type==IIC_TYPE_SIM)
    {
        if(iic_sim_write(dev->ops, byte, ack)<0)
            return -2;
    }
    else
    {
        
    }
    
    return 0;
}


int iic_bus_read(int fd)
{
    struct iic_dev *dev = iic_devs[fd];
    int ret = 0;
    
    if(dev==NULL)
        return -1;
    
    if(dev->iic_type==IIC_TYPE_SIM)
    {
        ret = iic_sim_read(dev->ops);
        if(ret<0)
            return -2;
    }
    else
    {
        
    }
    
    return ret;
}

int iic_bus_read_ack(int fd)
{
    struct iic_dev *dev = iic_devs[fd];
    int ret = 0;
    
    if(dev==NULL)
        return -1;
    
    if(dev->iic_type==IIC_TYPE_SIM)
    {
        ret = iic_sim_read_ack(dev->ops);
        if(ret<0)
            return -2;
    }
    else
    {
        
    }
    
    return ret; 
}



