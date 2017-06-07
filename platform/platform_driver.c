#include "platform_driver.h"

#include <string.h>

LIST_HEAD(platform_device_head);


int platform_driver_register(struct platform_device *dev)
{
    if(dev==NULL)
        return -1;
    
    list_add(&platform_device_head, &dev->entry);
    
    return 0;
}

int platform_driver_unregister(struct platform_device *dev)
{
    list_del(&dev->entry);
    return 0;
}

int open(const char *name, int flags)
{
    struct list_head *dev;
    struct list_head *tnode = NULL;
    
    list_for_each_safe(dev, tnode, &platform_device_head)
    {
        if(0!=strcmp(((struct platform_device *)dev)->name, name))
            continue;
        LOG_DEBUG("%s is opned!");
        return (int)dev;
    }
    
    return -1;
}

int close(int fd)
{
    
    return 0;
}

int write(int fd, char *datas, int len)
{
    struct device_operations *ops;
    struct platform_device *dev = (struct platform_device *)fd;
    char *pdata = datas;
    int ret = 0;
    int temp = 0;
    
    ops = dev->ops;
    
    while(len>0)
    {
        temp = ops->write(dev, pdata, len);
        if(temp<=0)
            break;
        len-=temp;
        pdata+=temp;
        ret+=temp;
    }
    
    return ret;
}

int read(int fd, char *buf, int len)
{
    struct device_operations *ops;
    struct platform_device *dev = (struct platform_device *)fd;
    char *pbuf = buf;
    int ret = 0;
    int temp = 0;
    
    ops = dev->ops;
    
    while(len>0)
    {
        temp = ops->read(dev, pbuf, len);
        if(temp<=0)
            break;
        len-=temp;
        pbuf+=temp;
        ret+=temp;
    } 
    
    return ret;
}

int ioctl(int fd, int cmd, int arg)
{
    struct device_operations *ops;
    struct platform_device *dev = (struct platform_device *)fd;
    
    ops = dev->ops;
    
    if(ops->ioctl(dev, cmd, arg)<0)
        return -1;
    
    return 0;
}

struct platform_device *platform_device_find_next(struct platform_device *current)
{
    struct list_head *entry;
    
    if(current==NULL)
        entry = &platform_device_head;
    else
        entry = &current->entry;
    
    entry = entry->next;
    
    if(entry == &platform_device_head)
        return NULL;

    return (struct platform_device *)entry;
}


