#ifndef __FLASH_H__
#define __FLASH_H__

#include "types.h"

enum{
    FLASHX_SECTOR_0,
    FLASHX_SECTOR_1,
    FLASHX_SECTOR_3,
    FLASHX_SECTOR_4,
    FLASHX_SECTOR_5,
    FLASHX_SECTOR_6,
    FLASHX_SECTOR_7,
    FLASHX_SECTOR_8,
    FLASHX_SECTOR_9,
    FLASHX_SECTOR_10,
    FLASHX_SECTOR_11,
    FLASHX_SECTOR_12,
};

typedef struct sector{
    e_uint8_t sector;
    e_uint8_t size;
}sector_t;

typedef enum{
    SECTOR_NORMAL,
    SECTOR_32K,
    SECTOR_64K,
}sector_type_t;


struct flash{
    const sector_t *sectors;
    const struct flash_operations *ops;
};

struct flash_operations{
    int (*open)(struct flash *flash, int flag);
    int (*write)(struct flash *flash, void *buffer, e_size_t size);
    int (*read)(struct flash *flash, void *buffer, e_size_t size);
    int (*erase)(struct flash *flash, sector_t sector, sector_type_t type);
};

e_err_t flash_register(int fd, struct flash *flash);
int flash_write(int fd, void *buffer, e_size_t size);
int flash_read(int fd, void *buffer, e_size_t size);
int flash_erase(int fd, sector_t sector, sector_type_t type);

#endif