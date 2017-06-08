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

static e_size_t dma_cal_recved_len(struct serial_dev *dev)
{
    static e_size_t rx_length;
    struct serial_rx_fifo *fifo = (struct serial_rx_fifo *)dev->serial_rx;
    
    ASSERT_PARAM(rx_fifo != NULL);
    
    rx_length = (rx_fifo->put_index >= rx_fifo->get_index)?(rx_fifo->put_index-rx_fifo->get_index):
                    (dev->config.bufsz - (rx_fifo->get_index-rx_fifo->put_index));
                    
    return rx_length;
}

static void dma_recv_update_get_index(struct serial_dev *dev, e_size_t len)
{
    struct serial_rx_fifo *rx_fifo = (struct serial_rx_fifo *)dev->serial_rx;
    
    ASSERT_PARAM(rx_fifo != NULL);
    ASSERT_PARAM(len<=dma_calc_recved_len(dev));
    
    rx_fifo->get_index += len;
    
    if(rx_fifo->get_index > dev->config.bufsz)
    {
        rx_fifo->get_index -= dev->config.bufsz;
    }
}

static void dma_recv_update_put_index(struct serial_dev *dev, e_size_t len)
{
    struct serial_rx_fifo *rx_fifo = (struct serial_rx_fifo *)serial->serial_rx;
    e_size_t i;
    
    ASSERT_PARAM(rx_fifo != NULL);
    
    if(rx_fifo->get_index <= rx_fifo->put_index)
    {
        rx_fifo->put_index += len;
        
        if(rx_fifo->put_index >= dev->config.bufsz)
        {
            for(i=0;i<len/dev->config.bufsz;i++)
            {
                rx_fifo->put_index -= dev->config.bufsz;
            }
            if(rx_fifo->put_index >= rx_fifo->get_index){
                rx_fifo->get_index = rx_fifo->put_index+1;
            }
        }
    }
    else
    {
        rx_fifo->put_index += len;
        if(rx_fifo->put_index >= rx_fifo->get_index)
        {
            if(rx_fifo->put_index >= dev->config.bufsz)
            {
                for(i=0;i<len/serial->config.bufsz;i++)
                {
                    rx_fifo->put_index -= dev->config.bufsz;
                }
            }
            rx_fifo->get_index = rx_fifo->put_index+1;
        }
    }
}

e_inline int _serial_dma_rx(struct serial_dev *dev, e_uint8_t *data, int length)
{
    ASSERT_PARAM((dev!=NULL)&&(data!=NULL));
    
    if(dev->config.bufsz==0)
    {
        int result = E_EOK;
        struct serial_rx_dma *rx_dma;
        rx_dma = (struct serial_rx_dma *)dev->serial_rx;
        ASSERT_PARAM(rx_dma != NULL);
        
        if(rx_dma->activated != E_TRUE)
        {
            rx_dma->activated = E_TRUE;
            ASSERT_PARAM(dev->ops->dma_transmit != NULL);
            dev->ops->dma_transmit(dev, data, length, SERIAL_DMA_RX);
        }
        else
            result = - E_EBUSY;
        
        if(result == E_EOK)
            return length;
        return 0;
    }
    else
    {
        struct serial_rx_fifo *rx_fifo = (struct serial_rx_fifo *)dev->serial_rx;
        e_size_t recv_len = 0;
        e_size_t fifo_recved_len = dma_calc_recved_len(dev);
        
        ASSERT_PARAM(rx_fifo != NULL);
        if(length < fifo_recved_len)
        {
            recv_len = length;
        }
        else
        {
            recv_len = fifo_recved_len;
        }
        if(rx_fifo->get_index+recv_len<dev->config.bufsz)
        {
            memcpy(data, rx_fifo->buffer+rx_fifo->get_index, recv_len);
        }
        else
        {
            memcpy(data, rx_fifo->buffer+rx_fifo->get_index,
                    dev->config.bufsz - rx_fifo->get_index);
            memcpy(data+dev->config.bufsz-rx_fifo->get_index, rx_fifo->buffer,
                    recv_len + rx_fifo->get_index-dev->config.bufsz);
        }
        
        dma_recv_update_get_index(dev, recv_len);
        
        return recv_len;
    }
}

e_inline int _serial_dma_tx(struct serial_dev *dev, const e_uint8_t *data, int length)
{
    
    
    
    return 0;
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





