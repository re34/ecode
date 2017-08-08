#include "flash.h"
#include "ecode.h"

static struct flash *fflashs[FLASHn];


e_err_t flash_register(int fd, struct flash *flash)
{
    ASSERT_PARAM(fd<=FLASHn);

    fflashs[fd] = flash;
    
    ASSERT_PARAM(flash!=NULL);
    
    fflashs[fd]=flash;
    
    return E_EOK;
}

int flash_write(int fd, void *buffer, e_size_t size)
{
    ASSERT_PARAM(fd<=FLASHn);
    
    struct flash *flash = fflashs[fd];
    
    ASSERT_PARAM(flash!=NULL);
    
    flash=fflashs[fd];
    
    const struct flash_operations *ops = flash->ops;
    if(ops==NULL)
        return -E_ERROR;
    
    if(ops->write==NULL)
        return -E_ERROR;
    
    return ops->write(flash, buffer, size);
}

int flash_read(int fd, void *buffer, e_size_t size)
{
    ASSERT_PARAM(fd<=FLASHn);
    
    struct flash *flash = fflashs[fd];
    
    ASSERT_PARAM(flash!=NULL);
    
    flash=fflashs[fd];
    
    const struct flash_operations *ops = flash->ops;
    if(ops==NULL)
        return -E_ERROR;
    
    if(ops->read==NULL)
        return -E_ERROR;
    
    return ops->read(flash, buffer, size);
}

int flash_erase(int fd, sector_t sector, sector_type_t type)
{
    ASSERT_PARAM(fd<=FLASHn);
    
    struct flash *flash = fflashs[fd];
    
    ASSERT_PARAM(flash!=NULL);
    
    flash=fflashs[fd];
    
    const struct flash_operations *ops = flash->ops;
    if(ops==NULL)
        return -E_ERROR;
    
    if(ops->erase==NULL)
        return -E_ERROR;
    
    return ops->erase(flash, sector, type);
}