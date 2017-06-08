#include "device.h"


/**
 * This function registers a device driver with specified name.
 *
 * @param dev the pointer of device driver structure
 * @param name the device driver's name
 * @param flags the flag of device
 *
 * @return the error code, E_EOK on initialization successfully.
 */
e_err_t e_device_register(e_device_t dev,
                            const char *name,
                            e_uint16_t flags)
{
    if(dev==NULL)
        return -E_ERROR;
    
    if(e_device_find(name) != NULL)
        return -E_EOK;
    
    e_object_init(&(dev->parent), name);
    
    dev->flag = flags;
    
    
    return E_EOK;
}

e_err_t e_device_unregister(e_device_t dev)
{
    e_object_detach(&(dev->parent));
    
    return E_EOK;
}

e_device_t e_device_find(const char *name)
{
    struct e_object *object;
    e_device_t dev;
    
    object = e_object_find(name);
    if(object==NULL)
        return NULL;
    
    dev = contianer_of(object, struct e_device, parent);
    
    return dev;    
}

e_err_t e_device_init(e_device_t dev)
{
    e_err_t result = E_EOK;
    
    if(dev->init!=NULL)
    {
        if(!(dev->flag&DEVICE_FLAG_ACTIVATED))
        {
            result = dev->init(dev);
            if(result!=E_EOK)
            {
                LOG_DEBUG("to initialize device: %s failed. The error code is %d",\
                            dev->parent.name, result);
            }
            else
            {
                dev->flag |= DEVICE_FLAG_ACTIVATED;
            } 
        }
    }
    
    return result;
}

e_err_t e_device_open(e_device_t dev, e_uint16_t oflag)
{
    e_err_t result = E_EOK;
    

    if(!(dev->flag&DEVICE_FLAG_ACTIVATED))
    {
        if(dev->init!=NULL)
        {
            result = dev->init(dev);
            if(result!=E_EOK)
            {
                LOG_DEBUG("to initialize device: %s failed. The error code is %d",\
                            dev->parent.name, result);
                return result;
            } 
        }

        dev->flag |= DEVICE_FLAG_ACTIVATED;

    }
    
    if(dev->open!=NULL)
        result = dev->open(dev, oflag);
    
    if(result == E_EOK || result == -E_ENOSYS)
    {
        dev->open_flag = oflag | DEVICE_FLAG_OPEN;
    }
    
    
    return result;
}

e_err_t e_device_close(e_device_t dev)
{
    e_err_t result = E_EOK;
    
    
    return result;
}

e_size_t e_device_read(e_device_t dev,
                        e_offset_t pos,
                        void *buffer;
                        e_size_t size)
{
   
   
   return 0;
}

e_size_t e_device_write(e_device_t dev,
                        e_offset_t pos,
                        const void *buffer,
                        e_size_t size)
{
    
    
    return 0;
}

e_err_t e_device_control(e_device_t dev,
                            e_uint8_t cmd,
                            void *args)
{
    
    
    return E_EOK;
}
