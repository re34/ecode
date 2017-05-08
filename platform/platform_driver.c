#include "platform_driver.h"

#include <string.h>

struct platform_device *platform_devices[DEVICEn]={0};

int platform_driver_register(device_t dev, int minor_num, struct platform_device *pdev)
{
    int i;
    if(dev>=DEVICEn)
        return -1;
    pdev->major = dev;
    
    for(i=0; i<minor_num;i++)
    {
        pdev->file[i].major = dev;
        pdev->file[i].minor = i;
        platform_devices[dev+i]=pdev;
        LOG_INFO("%s%d is regstered!", pdev->name, i);
    }
    
    return 0;
}

int platform_driver_unregister(device_t dev, int minor_num)
{

    return 0;
}

int write(int fd, char *datas, int len)
{
    struct device_operations *ops;
    int ret=0;
    if(platform_devices[fd]==NULL||platform_devices[fd]->ops==NULL)
        return -1;
    ops = platform_devices[fd]->ops;
    
    ret = ops->write(platform_devices[fd]->file, datas, len);
    if(ret<0)
        return -2;
    
    return ret;
}

int read(int fd, char *buf, int len)
{
    struct device_operations *ops;
    int ret=0;
    if(platform_devices[fd]==NULL||platform_devices[fd]->ops==NULL)
        return -1;
    ops = platform_devices[fd]->ops;
    
    ret = ops->read(platform_devices[fd]->file, buf, len);
    if(ret<0)
        return -2;
    
    return ret;
}

int ioctl(int fd, int cmd, int arg)
{
    struct device_operations *ops;
    int ret=0;
    if(platform_devices[fd]==NULL||platform_devices[fd]->ops==NULL)
        return -1;
    ops = platform_devices[fd]->ops;
    
    ret = ops->ioctl(platform_devices[fd]->file, cmd, arg);
    if(ret<0)
        return -2;
    
    return 0; 
}
