#include "serial.h"
#include "ecode.h"


static struct serial_dev *devs[COMn]={NULL};



e_inline int _serial_poll_rx(struct e_serial_dev *dev, e_uint8_t *data, int length)
{
    int ch;
    int size;
    
    ASSERT_PARAM(dev !=NULL);
    size = length;
    
    while(length)
    {
        ch = serial->ops->getc(dev);
        if(ch == -1)
            break;
        data ++;
        length --;
        if(ch == '\n')
            break;
    }
    
    return size-length;    
}

e_inline int _serial_poll_tx(struct serial_dev *dev, const e_uint8_t *data, int length)
{
    int size;
    ASSERT_PARAM(dev != NULL);
    
    size = length;
    while(length)
    {
        if(*data == '\n' && (dev->parent.open_flag & DEVICE_FLAG_STREAM))
        {
            dev->ops->putc(dev, '\n');
        }
        
        dev->ops->putc(dev, *data);
        
        ++data;
        --length;
    }
    
    return size-length;
}

e_inline int _serial_int_rx(struct serial_dev *dev, e_uint8_t *data, int length)
{
    int size;
    struct serial_rx_fifo *rx_fifo;
    
    ASSERT_PARAM(dev != NULL);
    
    size = length;
    
    rx_fifo = (struct serial_rx_fifo*) dev->serial_rx;
    ASSERT_PARAM(rx_fifo != NULL);
    
    while(length)
    {
        int ch;
        
        if(rx_fifo->get_index!=rx_fifo->put_index)
        {
            ch = rx_fifo->buffer[rx_fifo->get_index];
            rx_fifo->get_index += 1;
            if(rx_fifo->get_index >= dev->config.bufsz)
                rx_fifo->get_index = 0;
        }
        else
        {
            break;
        }
        
        *data = ch &0xff;
        data ++;
        length --;
                
    }
    
    return size - length;
}

e_inline int _serial_int_tx(struct serial_dev *dev, const e_uint8_t *data, int length)
{
    int size;
    struct serial_tx_fifo *tx;
    
    ASSERT_PARAM(dev != NULL);
    
    size = length;
    tx = (struct serial_tx_fifo *) dev->serial_tx;
    ASSERT_PARAM(tx!=NULL);
    
    while(length)
    {
        if(dev->ops->putc(dev, *(char *)data)==-1)
        {
            continue;
        }
        data ++;
        length --;
    }
    
    return size-length;
}

static e_err_t seiral_init(struct e_device *dev)
{
    e_err_t result = E_EOK;
    struct serial_device *serial;
    
    ASSERT_PARAM(dev!=NULL);
    
    serial = contianer_of(dev, struct serial_dev, parent);
    
    if(serial->ops->configure)
        result = serial->ops->configure(serial, &serial->config);
    
    return result;    
}


static int serial_dev_read(struct e_device *dev,
                    e_offset_t pos,
                    void *buffer,
                    e_size_t size)
{
    struct serial_dev *serial;
    
    ASSERT_PARAM(dev !=NULL);
    
    if(size == 0)
        return 0;
    
    serial = contianer_of(dev, struct serial_dev, parent);
    
    if(dev->open_flag&DEVICE_FLAG_INT_RX)
    {
        return _serial_int_rx(serial, buffer, size);
    }
    
    return _serial_poll_rx(serial, buffer, size);
}

static int serial_dev_write(struct e_device *dev,
                            e_offset_t pos,
                            const void *buffer,
                            e_size_t size)
{
    struct serial_dev *serial;
    
    ASSERT_PARAM(dev!=NULL);
    
    if(size==0)
        return 0;
    serial = contianer_of(dev, struct serial_dev, parent);
    
    if(dev->open_flag&DEVICE_FLAG_INT_TX)
    {
        return _serial_int_tx(serial, buffer, size);
    }
    
    return _serial_poll_tx(serial, buffer, size);
}

static e_err_t serial_dev_control(struct e_device *dev,
                                    e_uint8_t cmd,
                                    void *args)
{
    struct serial_dev *serial;
    ASSERT_PARAM(dev!=NULL);
    
    serial = contianer_of(dev, struct serial_dev, parent);
    
    switch(cmd)
    {
    case DEVICE_CONFIG:
        if(args)
        {
            serial->config = *(struct serial_configure *)args;
            
            serial->ops->configure(serial, (struct serial_configure *)args);
        }
        break;
    default:
        serial->ops->control(serial, cmd, args);
        break;
    }
    
    return E_EOK;
}


e_err_t serial_hw_register(struct serial_dev *serial,
                            const char *name,
                            e_uint32_t flag,
                            void *data)
{
    struct e_device *device;
    ASSERT_PARAM(serial!=NULL);
    
    device = &(serial->parent);
    
    device->init = NULL;
    device->open = NULL;
    device->close = NULL;
    device->read = serial_dev_read;
    device->write = serial_dev_write;
    device->control = serial_dev_control;
    device->private_data = data;
    
    
    return e_device_register(device, name, flag);
}


int serial_register(int fd,
                    struct serial_dev *dev,
                    const char *name, 
                    e_uint32_t flag,
                    void *data)
{
    int ret;
    
    ASSERT_PARAM((fd<COMn)&&(dev!=NULL));
    
    devs[fd]=dev;
    
    ret = serial_hw_register(dev, name, flag, data)
    
    if(ret<0)
        return -1;
    LOG_DEBUG("COM%d is registered!", fd+1);
    return 0;
}


int serial_write(int fd,
                e_offset_t pos,
                const void *buffer,
                e_size_t size)
{
    int ret=0;
    struct serial_dev *dev;
    
    ASSERT_PARAM(fd<COMn);
    
    dev = devs[fd];
    
    ASSERT_PARAM(dev!=NULL);
    
    return serial_dev_write(dev, pos, buffer, size)
}

int serial_read(int fd, 
                e_offset_t pos,
                void *buffer,
                e_size_t size)
{
    int ret=0;
    struct serial_dev *dev;
    
    ASSERT_PARAM(fd<COMn);
    
    dev = devs[fd];
    
    ASSERT_PARAM(dev!=NULL);
    
    return serial_dev_read(dev, pos, buffer, size);
}

int serial_in_waiting(int fd)
{
    struct serial_dev *dev;

    return 0;
}





