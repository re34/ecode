#include "firmware.h"
#include "ecode.h"

struct firmware *_firmware;

e_err_t firmware_register(struct firmware *firmware)
{
    ASSERT_PARAM(firmware!=NULL);
    ASSERT_PARAM(firmware->ops!=NULL);
    
    _firmware = firmware;
    
    return E_EOK;
}

e_err_t firmware_upgrade(void)
{
    const struct firmware_operations *ops;
    ASSERT_PARAM(_firmware!=NULL);
    ASSERT_PARAM(_firmware->ops!=NULL);
    ops = _firmware->ops;
    if(ops->update==NULL)
        return -E_ERROR;
    return ops->update(_firmware);
}

e_err_t firmware_open(struct firmware *firmware)
{
    const struct firmware_operations *ops;
    ASSERT_PARAM(firmware!=NULL);
    ASSERT_PARAM(firmware->ops!=NULL);
    
    ops = firmware->ops;
    
    if(ops->open==NULL)
        return 0;
    
    ops->open();
    
    return E_EOK;
}

void firmware_close(struct firmware *firmware)
{
    const struct firmware_operations *ops;
    ASSERT_PARAM(firmware!=NULL);
    ASSERT_PARAM(firmware->ops!=NULL);
    
    ops = firmware->ops;
    
    if(ops->close==NULL)
        return;
    
    ops->close();
    
}

int firmware_write(struct firmware *firmware, e_uint32_t address, void *buffer, e_size_t size)
{
    const struct firmware_operations *ops;
    ASSERT_PARAM(firmware!=NULL);
    ASSERT_PARAM(firmware->ops!=NULL);
    
    ops = firmware->ops;
    
    if(ops->write==NULL)
        return 0;
    
    return ops->write(address, buffer, size);
}


e_err_t firmware_erase(struct firmware *firmware)
{
    const struct firmware_operations *ops;
    ASSERT_PARAM(firmware!=NULL);
    ASSERT_PARAM(firmware->ops!=NULL);
    
    ops = firmware->ops;
    
    if(ops->erase==NULL)
        return -E_ERROR;
    
    return ops->erase();
}
