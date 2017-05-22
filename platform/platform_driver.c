#include "platform_driver.h"

#include <string.h>

LIST_HEAD(platform_driver_head);

/*
int platform_driver_register(device_t dev, int minor_num, struct platform_device *pdev)
{
    int i;

    
    return 0;
}

int platform_driver_unregister(device_t dev, int minor_num)
{

    return 0;
}
*/

int open(const char *name)
{
    
    
    return 0;
}

int close(int fd)
{
    
    return 0;
}

int write(int fd, char *datas, int len)
{
    struct device_operations *ops;
    int ret=0;

    
    return ret;
}

int read(int fd, char *buf, int len)
{
    struct device_operations *ops;
    int ret=0;

    
    return ret;
}

int ioctl(int fd, int cmd, int arg)
{
    struct device_operations *ops;
    int ret=0;

    
    return 0; 
}
