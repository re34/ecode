#include "serial.h"
#include "ecode.h"


static struct serial_dev *serials[COMn]={NULL};



e_inline int _serial_poll_rx(struct serial_dev *serial, e_uint8_t *data, int length)
{
    int ch;
    int size;
    
    ASSERT_PARAM(serial!=NULL);
    size = length;
    
    while(length)
    {
        ch = serial->ops->getc(serial);
        if(ch == -1)
            break;
        data ++;
        length --;
        if(ch == '\n')
            break;
    }
    
    return size-length;    
}

e_inline int _serial_poll_tx(struct serial_dev *serial, const e_uint8_t *data, int length)
{
    int size;
    ASSERT_PARAM(serial != NULL);
    
    size = length;
    while(length)
    {
        if(*data == '\n' && (serial->parent.open_flag & DEVICE_FLAG_STREAM))
        {
            serial->ops->putc(serial, '\n');
        }
        
        serial->ops->putc(serial, *data);
        
        ++data;
        --length;
    }
    
    return size-length;
}


e_inline int _serial_int_rx(struct serial_dev *serial, e_uint8_t *data, int length)
{
    int size;
    struct serial_rx_fifo *rx_fifo;
    
    ASSERT_PARAM(serial != NULL);
    
    size = length;
    
    rx_fifo = (struct serial_rx_fifo*) serial->serial_rx;
    ASSERT_PARAM(rx_fifo != NULL);
    
    while(length)
    {
        int ch;
        
        if(rx_fifo->get_index!=rx_fifo->put_index)
        {
            ch = rx_fifo->buffer[rx_fifo->get_index];
            rx_fifo->get_index += 1;
            if(rx_fifo->get_index >= serial->config.bufsz)
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

e_inline int _serial_int_tx(struct serial_dev *serial, const e_uint8_t *data, int length)
{
    int size;
    struct serial_tx_fifo *tx;
    
    ASSERT_PARAM(serial != NULL);
    
    size = length;
    tx = (struct serial_tx_fifo *) serial->serial_tx;
    ASSERT_PARAM(tx!=NULL);
    
    while(length)
    {
        if(serial->ops->putc(serial, *(char *)data)==-1)
        {
            continue;
        }
        data ++;
        length --;
    }
    
    return size-length;
}



static e_err_t seiral_init(struct device *dev)
{
    e_err_t result = E_EOK;
    struct serial_dev *serial;
    
    ASSERT_PARAM(dev!=NULL);
    
    serial = contianer_of(dev, struct serial_dev, parent);
    
    if(serial->ops->configure)
        result = serial->ops->configure(serial, &serial->config);
    
    return result;    
}



static e_size_t serial_dev_read(struct device *dev,
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

static e_size_t serial_dev_write(struct device *dev,
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


static e_err_t serial_dev_control(struct device *dev,
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


e_err_t serial_dev_register(struct serial_dev *serial,
                            const char *name,
                            e_uint32_t flag,
                            void *data)
{
    struct device *device;
    ASSERT_PARAM(serial!=NULL);
    
    device = &(serial->parent);
    
    device->init = NULL;
    device->open = NULL;
    device->close = NULL;
    device->read = serial_dev_read;
    device->write = serial_dev_write;
    device->control = serial_dev_control;
    device->private_data = data;
    
    
    return device_register(device, name, flag);
}


e_err_t serial_register(int fd,
                    struct serial_dev *serial,
                    const char *name, 
                    e_uint32_t flag,
                    void *data)
{
    e_err_t err;
    ASSERT_PARAM((fd<COMn)&&(serial!=NULL));
    
    serials[fd]=serial;
    
    err = serial_dev_register(serial, name, flag, data);
    
    if(err!=E_EOK)
        return -E_ERROR;
    LOG_DEBUG("COM%d is registered!", fd+1);
    return E_EOK;
}


e_size_t serial_write(int fd,
                const void *buffer,
                e_size_t size)
{
    struct serial_dev *serial;
    
    ASSERT_PARAM(fd<COMn);
    
    serial = serials[fd];
    
    ASSERT_PARAM(serial!=NULL);
    
    return serial_dev_write(&(serial->parent), 0, buffer, size);
}

e_size_t serial_read(int fd, 
                void *buffer,
                e_size_t size)
{
    struct serial_dev *serial;
    
    ASSERT_PARAM(fd<COMn);
    
    serial = serials[fd];
    
    ASSERT_PARAM(serial!=NULL);
    
    return serial_dev_read(&(serial->parent), 0, buffer, size);
}

int serial_in_waiting(int fd)
{
    struct serial_dev *serial;

    return 0;
}

void serial_hw_isr(struct serial_dev *serial, int event)
{
    struct serial_rx_fifo *rx_fifo;
    switch(event&0xFF)
    {
        case SERIAL_EVENT_RX_IND:
        {
            int ch = -1;
            rx_fifo = (struct serial_rx_fifo *)serial->serial_rx;
            ASSERT_PARAM(rx_fifo!=NULL);
            
            while(1)
            {
                ch = serial->ops->getc(serial);
                if(ch == -1)
                    break;
                rx_fifo->buffer[rx_fifo->put_index]=ch;
                rx_fifo->put_index += 1;
                if(rx_fifo->put_index>=serial->config.bufsz)
                    rx_fifo->put_index = 0;
                if(rx_fifo->put_index == rx_fifo->get_index)
                {
                    rx_fifo->get_index += 1;
                    if(rx_fifo->put_index >= serial->config.bufsz)
                    {
                        rx_fifo->get_index = 0;
                    }
                }
            }
            if(serial->parent.rx_indicate!=NULL)
            {
                e_size_t rx_length;
                
                rx_length = (rx_fifo->put_index>=rx_fifo->get_index)?(rx_fifo->put_index-rx_fifo->put_index):
                        (serial->config.bufsz-(rx_fifo->get_index-rx_fifo->put_index));
                serial->parent.rx_indicate(&serial->parent, rx_length);
            }
            break;
        }
        case SERIAL_EVENT_TX_DONE:
        {
            struct serial_tx_fifo *tx_fifo;
            
            tx_fifo = (struct serial_tx_fifo *)serial->serial_tx;
            //completion_done(&(tx_fifo->ccompletion));
            break;
        }
    }
}



