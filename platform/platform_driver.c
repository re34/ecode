#include "platform_driver.h"

#include <string.h>

LIST_HEAD(platform_device_head);

static struct platform_device * devs[DEVICEn]={NULL};


int platform_driver_register(device_name_t name,struct platform_device *dev)
{
    if(dev==NULL)
        return -1;
    
    if(devs[name]!=NULL)
        return -2;
    
    devs[name] = dev;
    list_add(&platform_device_head, &dev->entry);
    
    LOG_DEBUG("%s is registered!", dev->name);
    
    return 0;
}

int platform_driver_unregister(device_name_t name)
{

    if(devs[name]!=NULL)
    {
        list_del(&devs[name]->entry);
        LOG_DEBUG("%s is unregistered!", devs[name]->name);
    }
    devs[name]=NULL;
    
    return 0;
}

int open(const char *name, int flags)
{
    int i;
    int ret;
    struct platform_device *dev = NULL;
    struct device_operations *ops = NULL;
    
    for(i=0;i<DEVICEn;i++)
    {
        dev = devs[i];
        if(dev==NULL)
            continue;
        if(0==strcmp(name, dev->name))
        {
            if(dev->ops!=NULL)
            {
                ops = dev->ops;
                if(ops->open==NULL)
                {
                    LOG_ERROR("%s has no operation named open!", name);
                    return -1;
                }
                ret = ops->open(dev, flags);
                if(ret<0)
                    return -2;
                else
                    return i;
            }
            else
            {
                LOG_ERROR("%s has no operation!", name);
                return -3;
            }
        }
    }
    
    LOG_ERROR("%s can not found!", name);
    
    return -4;
}

int close(int fd)
{
    int i;
    int ret;
    struct platform_device *dev = NULL;
    struct device_operations *ops = NULL;
    
    for(i=0;i<DEVICEn;i++)
    {
        dev = devs[i];
        if(dev==NULL)
            continue;

            if(dev->ops!=NULL)
            {
                ops = dev->ops;
                if(ops->close==NULL)
                {
                    LOG_ERROR("%d has no operation named close!", fd);
                    return -1;
                }
                ret = ops->close(dev);
                if(ret<0)
                    return -2;
                else
                    return i;
            }
            else
            {
                LOG_ERROR("%d has no operation!", fd);
                return -3;
            }
        
    }
    
    LOG_ERROR("%d can not found!", fd);
    
    return -4;
}

int write(int fd, char *datas, int len)
{
    struct device_operations *ops;
    struct platform_device *dev;  
    char *pdata = datas;
    
    int ret = 0;
    int temp = 0;
    
    
    dev = devs[fd];
    
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
    struct platform_device *dev;
    char *pbuf = buf;
    int ret = 0;
    int temp = 0;
    
    dev = devs[fd];
    
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
    struct platform_device *dev;
    
    
    dev = devs[fd];
    
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


