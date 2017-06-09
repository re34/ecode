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
    
    return size->length;
}


int serial_register(int fd, struct serial_dev *dev)
{
    if(fd>=COMn)
        return -1;
    if(dev==NULL)
        return -2;
    
    devs[fd]=dev;
    
    LOG_DEBUG("COM%d is registered!", fd+1);
    return 0;
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
        
    }
    
    return E_EOK;
}

int serial_write(int fd, char *datas, int len)
{
    int ret=0;
    struct serial_dev *dev;
    
    if(fd>=COMn)
        return -1;
    dev = devs[fd];
    if(dev==NULL)
        return -2;
    
    while(len!=0)
    {
        if(dev->put_c(*datas++)<0)
            return -2;
        len--;
        ret++;
    }

    return ret;
}

int serial_read(int fd, char *buf, int len)
{
    int ret=0;
    struct serial_dev *dev;
    
    if(fd>=COMn)
        return -1;
    dev = devs[fd];
    if(dev==NULL)
        return -2;
    
    while(len!=0)
    {
        *buf++=dev->get_c();
        len--;
        ret++;
    }
    
    return ret;
}

int serial_in_waiting(int fd)
{
    struct serial_dev *dev;
    
    if(fd>=COMn)
        return -1;
    
    dev = devs[fd];
    
    if(dev==NULL)
        return -2;
    
    return dev->in_waiting();
}





