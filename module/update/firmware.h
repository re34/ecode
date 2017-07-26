#ifndef __FIRMWARE_H__
#define __FIRMWARE_H__
#include "types.h"

struct firmware{    
    const struct firmware_operations *ops;
};

struct firmware_operations{
    e_err_t (*erase)(void);
    int (*write)(e_uint32_t address, void *buffer, e_size_t size);
    e_err_t (*update)(struct firmware *firmware);
};

e_err_t firmware_register(struct firmware *firmware);
e_err_t firmware_upgrade(void);
int firmware_write(struct firmware *firmware, e_uint32_t address, void *buffer, e_size_t size);
e_err_t firmware_erase(struct firmware *firmware);
#endif