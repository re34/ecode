#include "serial.h"
#include "assert.h"
#include "print_log.h"

#ifndef COMn
#define COMn    2
#endif

static struct serial *serials[COMn]={NULL};

e_inline int _serial_poll_rx(struct serial *serial, e_uint8_t *data, int length)
{
    int ch;
    int size;
    
    ASSERT_PARAM(serial!=NULL);
    size = length;
    
    while(length)
    {
        ch = serial->ops->fgetc(serial);
        if(ch == -1)
            break;
        data ++;
        length --;
        if(ch == '\n')
            break;
    }
    
    return size-length;    
}

e_inline int _serial_poll_tx(struct serial *serial, const e_uint8_t *data, int length)
{
    int size;
    ASSERT_PARAM(serial != NULL);
    
    size = length;
    while(length)
    {
        serial->ops->fputc(serial, *data);
        
        ++data;
        --length;
    }
    
    return size-length;
}


e_inline int _serial_int_rx(struct serial *serial, e_uint8_t *data, int length)
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

e_inline int _serial_int_tx(struct serial *serial, const e_uint8_t *data, int length)
{
    int size;
    struct serial_tx_fifo *tx;
    
    ASSERT_PARAM(serial != NULL);
    
    size = length;
    tx = (struct serial_tx_fifo *) serial->serial_tx;
    ASSERT_PARAM(tx!=NULL);
    
    while(length)
    {
        if(serial->ops->fputc(serial, *(char *)data)==-1)
        {
            continue;
        }
        data ++;
        length --;
    }
    
    return size-length;
}

e_err_t serial_register(int fd,
                    struct serial *serial,
                    const char *name)
{
    e_err_t err;
    const struct serial_operation *ops;
    
    ASSERT_PARAM((fd<COMn)&&(serial!=NULL));
    ops = serial->ops;
    ASSERT_PARAM(ops!=NULL);
    
    err = ops->init(serial);
    
    if(err!=E_EOK)
        return -E_ERROR;
    

    serial->sem = os_sem_create();
    
    os_sem_release(serial->sem);
    
    serials[fd]=serial;
    
    LOG_DEBUG("%s is registered!", name);
    
    return E_EOK;
}

struct serial * serial_open(int fd, int oflag)
{
    struct serial *serial;
    
    ASSERT_PARAM(fd<COMn);
    
    serial = serials[fd];
    
    ASSERT_PARAM(serial!=NULL);
    
    os_sem_wait(serial->sem, OS_WAIT_FOREVER);
    
    return serial;
}

void serial_close(struct serial *serial)
{
    if(serial==NULL)
        return;
    
    os_sem_release(serial->sem);
}

e_size_t serial_write(struct serial *serial,
                const void *buffer,
                e_size_t size)
{
    e_size_t ret = 0;
    
    if(serial==NULL)
        return 0;

    if(serial->flag&SERIAL_FLAG_INT_TX)
    {
        ret =  _serial_int_tx(serial, buffer, size);
    }
    else
    {
        ret = _serial_poll_tx(serial, buffer, size);
    }
    
    return ret;
}

e_size_t serial_read(struct serial *serial, 
                void *buffer,
                e_size_t size)
{
    e_size_t ret = 0;
    
    if(serial==NULL)
        return 0;
    
    if(serial->flag&SERIAL_FLAG_INT_RX)
    {
        ret = _serial_int_rx(serial, buffer, size);
    }
    else
    {
        ret = _serial_poll_rx(serial, buffer, size);
    }
    
    return ret;
}



void serial_hw_isr(struct serial *serial, int event)
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
                ch = serial->ops->fgetc(serial);
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

            break;
        }
        case SERIAL_EVENT_TX_DONE:
        {
            struct serial_tx_fifo *tx_fifo;
            
            tx_fifo = (struct serial_tx_fifo *)serial->serial_tx;

            break;
        }
    }
}



