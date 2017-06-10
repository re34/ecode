#ifndef __DEVICE_H__
#define __DEVICE_H__
#include "object.h"


#define DEVICE_FLAG_ACTIVATED       BIT(0)
#define DEVICE_FLAG_OPEN            BIT(1)
#define DEVICE_FLAG_CLOSE           BIT(2)
#define DEVICE_FLAG_STREAM          BIT(3)
#define DEVICE_FLAG_INT_RX          BIT(4)
#define DEVICE_FLAG_INT_TX          BIT(5)



typedef struct device * device_t;

struct device
{
    struct e_object parent;
    e_uint16_t flag;
    e_uint16_t open_flag;
    /* device call back*/
    rt_err_t (*rx_indicate)(rt_device_t dev, rt_size_t size);
    rt_err_t (*tx_complete)(rt_device_t dev, void *buffer);
    
    e_err_t (*init)(device_t dev);
    e_err_t (*open)(device_t dev, e_uint16_t oflag);
    e_err_t (*close)(device_t dev);
    e_size_t (*read)(device_t dev, e_offset_t pos, void *buffer, e_size_t size);
    e_size_t (*write)(device_t dev, e_offset_t pos, const void *buffer, int size);
    e_err_t (*control)(device_t dev, e_uint8_t cmd, void *args);
    
    void *private_data; //private data
};



e_err_t device_register(device_t dev,
                            const char *name,
                            e_uint16_t flags);
e_err_t device_unregister(device_t dev);
device_t device_find(const char *name);
e_err_t device_init(device_t dev);
e_err_t device_open(device_t dev, e_uint16_t oflag);
e_err_t device_close(device_t dev);
e_size_t device_read(device_t dev,
                        e_offset_t pos,
                        void *buffer,
                        e_size_t size);
e_size_t device_write(device_t dev,
                        e_offset_t pos,
                        const void *buffer,
                        e_size_t size);
e_err_t device_control(device_t dev,
                            e_uint8_t cmd,
                            void *arg);

#endif