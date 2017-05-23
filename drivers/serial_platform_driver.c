#include "serial_platform_driver.h"
#include "ecode.h"

static int serial_platform_read(struct platform_device *dev, char *buf, int len);
static int serial_platform_write(struct platform_device *dev, char *datas, int len);
static int serial_platform_ioctl(struct platform_device *dev, int cmd, int arg);

static struct device_operations serial_ops={
    .read = serial_platform_read,
    .write = serial_platform_write,
    .ioctl = serial_platform_ioctl,
};

static struct platform_device serial1_dev={
    .minor = COM1,
    .name = "COM1",
    .ops = &serial_ops,
};

static struct platform_device serial2_dev={
    .minor = COM2,
    .name = "COM2",
    .ops = &serial_ops,
};

int serial_platform_init()
{
    //串口已在驱动层初始化过，故不再这里进行初始化设置了
    if(platform_driver_register(&serial1_dev)<0)
        return -1;
    if(platform_driver_register(&serial2_dev)<0)
        return -2;
    
    return 0;
}



static int serial_platform_read(struct platform_device *dev, char *buf, int len)
{
    int ret = 0;
    
    ret = serial_read(dev->minor, buf, len);
    
    return ret;
}

static int serial_platform_write(struct platform_device *dev, char *datas, int len)
{
    int ret = 0;
    
    ret = serial_write(dev->minor, datas, len);
    
    return ret;    
}

static int serial_platform_ioctl(struct platform_device *dev, int cmd, int arg)
{
    
    return 0;
}