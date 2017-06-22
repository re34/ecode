#include "device.h"


/**
 * This function registers a device driver with specified name.
 *
 * @param dev the pointer of device driver structure
 * @param name the device driver's name
 * @param flags the flag of device
 *
 * @return the error code, E_EOK on successfully.
 */
e_err_t device_register(device_t dev,
                            const char *name,
                            e_uint16_t flags)
{
    if(dev==NULL)
        return -E_ERROR;
    
    if(device_find(name) != NULL)
        return -E_EOK;
    
    object_init(&(dev->parent), name);
    
    dev->flag = flags;
    
    
    return E_EOK;
}
/**
 * This function unregisters a device driver.
 *
 * @param dev the pointer of device driver structure
 * @param flags the flag of device
 *
 * @return the error code, E_EOK on successfully.
 */
e_err_t device_unregister(device_t dev)
{
    object_detach(&(dev->parent));
    
    return E_EOK;
}
/**
 * This function find a device with name.
 *
 * @param name the device name to find
 *
 * @return the device handle, NULL on no devcie found.
 */
device_t device_find(const char *name)
{
    struct object *object;
    device_t dev;
    
    object = object_find(name);
    if(object==NULL)
        return NULL;
    
    dev = container_of(object, struct device, parent);
    
    return dev;    
}
/**
 * This function init a device driver.
 *
 * @param dev the pointer of device driver structure
 *
 * @return the error code, E_EOK on initialization successfully.
 */
e_err_t device_init(device_t dev)
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
/**
 * This function will open a device.
 *
 * @param dev the pointer of device driver structure
 * @param flags the flag of device open
 *
 * @return the error code, E_EOK on successfully.
 */
e_err_t device_open(device_t dev, e_uint16_t oflag)
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
/**
 * This function will close a device.
 *
 * @param dev the pointer of device driver structure
 *
 * @return the error code, E_EOK on successfully.
 */
e_err_t device_close(device_t dev)
{
    e_err_t result = E_EOK;
    
    if(dev->close!=NULL)
        result = dev->close(dev);
    
    if(result==E_EOK||result==-E_ENOSYS)
        dev->open_flag = DEVICE_FLAG_CLOSE;
    
    
    return result;
}
/**
 * This function will read some data from a device.
 *
 * @param dev the pointer of device driver structure
 * @param pos the position of reading
 * @param buffer the data buffer to save read data
 * @param size the size of buffer
 *
 * @return the actually read size on successful, otherwise negative returned.
 */
e_size_t device_read(device_t dev,
                        e_offset_t pos,
                        void *buffer,
                        e_size_t size)
{
   
   
   if(dev->read!=NULL)
       return dev->read(dev, pos, buffer, size);
   
   return 0;
}
/**
 * This function will write some data to a device.
 *
 * @param dev the pointer of device driver structure
 * @param pos the position of written
 * @param buffer the data buffer to be written to device
 * @param size the size of buffer
 *
 * @return the actually written size on successful, otherwise negative returned.
 */
e_size_t device_write(device_t dev,
                        e_offset_t pos,
                        const void *buffer,
                        e_size_t size)
{
    
    if(dev->write != NULL)
        return dev->write(dev, pos, buffer, size);
    
    return 0;
}
/**
 * This function will perform a variety of control functions on devices.
 *
 * @param dev the pointer of device driver structure
 * @param cmd the command sent to device
 * @param arg the argument of command
 *
 * @return the result.
 */
e_err_t device_control(device_t dev,
                            e_uint8_t cmd,
                            void *arg)
{
    
    if(dev->control != NULL)
        return dev->control(dev, cmd, arg);
    
    return E_EOK;
}
