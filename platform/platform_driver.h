#ifndef __PLATFROM_DRIVER_H__
#define __PLATFROM_DRIVER_H__
#include "includes.h"


struct platform_device{
    struct list_head entry;
    const char *name;
    int major;
    int minor;
    void *ops;
};


struct device_operations{
    int (*read)(struct platform_device *dev, char *buf, int len);
    int (*write)(struct platform_device *dev, char *datas, int len);
    int (*ioctl)(struct platform_device *dev, int cmd, int arg);
};


int platform_driver_register(struct platform_device *dev, const char *name);
int platform_driver_unregister(struct platform_device *dev);
int open(const char *name, int flags);
int close(int fd);
int write(int fd, char *datas, int len);
int read(int fd, char *buf, int len);
int ioctl(int fd, int cmd, int arg);
struct platform_device *platform_device_find_next(struct platform_device *current);
#endif