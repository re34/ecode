#ifndef __PLATFROM_DRIVER_H__
#define __PLATFROM_DRIVER_H__
#include "includes.h"

struct device_operations{
    int (*read)(char *buf, int len);
    int (*write)(char *datas, int len);
    int (*ioctl)(int cmd, int arg);
};


struct platform_device{
    struct list_head head;
    const char *name;
    UInt8 major;
    UInt8 minor_num;
    struct device_operations *ops;
};


//int platform_driver_register(device_t dev, int minor_num, struct platform_device *pdev);
//int platform_driver_unregister(device_t dev, int minor_num);
int open(const char *name);
int close(int fd);
int write(int fd, char *datas, int len);
int read(int fd, char *buf, int len);
int ioctl(int fd, int cmd, int arg);

#endif