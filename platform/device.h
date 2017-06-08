#ifndef __DEVICE_H__
#define __DEVICE_H__
#include "object.h"


#define DEVICE_FLAG_ACTIVATED       BIT(0)
#define DEVICE_FLAG_OPEN            BIT(1)
#define DEVICE_FLAG_CLOSE           BIT(2)
#define DEVICE_FLAG_STREAM          BIT(3)


typedef struct e_device * e_device_t;

struct e_device
{
    struct e_object parent;
    e_uint16_t flag;
    e_uint16_t open_flag;
    /* device call back*/
    
    
    e_err_t (*init)(e_device_t dev);
    e_err_t (*open)(e_device_t dev, e_uint16_t oflag);
    e_err_t (*close)(e_device_t dev);
    e_size_t (*read)(e_device_t dev, e_offset_t pos, void *buffer, e_size_t size);
    e_size_t (*write)(e_device_t dev, e_offset_t pos, const void *buffer, int size);
    e_err_t (*control)(e_device_t dev, e_uint8_t cmd, void *args);
    
    void *private_data; //private data
};



e_err_t e_device_register(e_device_t dev,
                            const char *name,
                            e_uint16_t flags);
e_err_t e_device_unregister(e_device_t dev);
e_device_t e_device_find(const char *name);
e_err_t e_device_init(e_device_t dev);
e_err_t e_device_open(e_device_t dev, e_uint16_t oflag);
e_err_t e_device_close(e_device_t dev);
e_size_t e_device_read(e_device_t dev,
                        e_offset_t pos,
                        void *buffer,
                        e_size_t size);
e_size_t e_device_write(e_device_t dev,
                        e_offset_t pos,
                        const void *buffer,
                        e_size_t size);
e_err_t e_device_control(e_device_t dev,
                            e_uint8_t cmd,
                            void *arg);

#endif