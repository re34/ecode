#ifndef __PLATFROM_DRIVER_H__
#define __PLATFROM_DRIVER_H__
#include "includes.h"
#include "platform_device_tree.h"

typedef int dev_t;


struct device_file{
    UInt8 major;
    UInt8 minor;
    void *content;
};

struct device_operations{
    int (*read)(struct device_file *file, char *buf, int len);
    int (*write)(struct device_file *file, char *datas, int len);
    int (*ioctl)(struct device_file *file, int cmd, int arg);
    int (*flush)(struct device_file *file);
};


struct platform_device{
    const char *name;
    UInt8 major;
    UInt8 minor_num;
    struct device_file *file;
    struct device_operations *ops;
};


int platform_driver_register(dev_t dev, int minor_num, struct platform_device *pdev);
int platform_driver_unregister(dev_t dev, int minor_num);
int write(int fd, char *datas, int len);
int read(int fd, char *buf, int len);
int ioctl(int fd, int cmd, int arg);

#endif