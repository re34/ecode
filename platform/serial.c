#include "serial.h"


static struct serial_dev *devs[COMn]={NULL};

int serial_register(int fd, struct serial_dev *dev)
{
    if(fd>=COMn)
        return -1;
    if(dev==NULL)
        return -2;
    
    devs[fd]=dev;
    
    LOG_DEBUG("COM%d is registered!", fd+1);
    return 0;
}

int serial_write(int fd, char *datas, int len)
{
    int ret=0;
    struct serial_dev *dev;
    
    if(fd>=COMn)
        return -1;
    dev = devs[fd];
    if(dev==NULL)
        return -2;
    
    while(len!=0)
    {
        if(dev->put_c(*datas++)<0)
            return -2;
        len--;
        ret++;
    }

    return ret;
}

int serial_read(int fd, char *buf, int len)
{
    int ret=0;
    struct serial_dev *dev;
    
    if(fd>=COMn)
        return -1;
    dev = devs[fd];
    if(dev==NULL)
        return -2;
    
    while(len!=0)
    {
        *buf++=dev->get_c();
        len--;
        ret++;
    }
    
    return ret;
}

int serial_in_waiting(int fd)
{
    struct serial_dev *dev;
    
    if(fd>=COMn)
        return -1;
    
    dev = devs[fd];
    
    if(dev==NULL)
        return -2;
    
    return dev->in_waiting();
}





