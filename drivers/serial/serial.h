#ifndef __SERIAL_H__
#define __SERIAL_H__
#include "device.h"
#include "rtos.h"

#define SERIAL_OFLAG_WR             0x01
#define SERIAL_OFLAG_RD             0x02

#define SERIAL_FLAG_STREAM          BIT(1)
#define SERIAL_FLAG_INT_RX          BIT(2)
#define SERIAL_FLAG_INT_TX          BIT(3)

#define BAUD_RATE_2400                  2400
#define BAUD_RATE_4800                  4800
#define BAUD_RATE_9600                  9600
#define BAUD_RATE_19200                 19200
#define BAUD_RATE_38400                 38400
#define BAUD_RATE_57600                 57600
#define BAUD_RATE_115200                115200
#define BAUD_RATE_230400                230400
#define BAUD_RATE_460800                460800
#define BAUD_RATE_921600                921600
#define BAUD_RATE_2000000               2000000
#define BAUD_RATE_3000000               3000000

#define DATA_BITS_5                     5
#define DATA_BITS_6                     6
#define DATA_BITS_7                     7
#define DATA_BITS_8                     8
#define DATA_BITS_9                     9

#define STOP_BITS_1                     0
#define STOP_BITS_2                     1
#define STOP_BITS_3                     2
#define STOP_BITS_4                     3

#define PARITY_NONE                     0
#define PARITY_ODD                      1
#define PARITY_EVEN                     2

#define BIT_ORDER_LSB                   0
#define BIT_ORDER_MSB                   1

#define NRZ_NORMAL                      0       /* Non Return to Zero : normal mode */
#define NRZ_INVERTED                    1       /* Non Return to Zero : inverted mode */

#ifndef SERIAL_RB_BUFSZ
#define SERIAL_RB_BUFSZ              64
#endif

#define SERIAL_EVENT_RX_IND          0x01    /* Rx indication */
#define SERIAL_EVENT_TX_DONE         0x02    /* Tx complete   */
#define SERIAL_EVENT_RX_DMADONE      0x03    /* Rx DMA transfer done */
#define SERIAL_EVENT_TX_DMADONE      0x04    /* Tx DMA transfer done */
#define SERIAL_EVENT_RX_TIMEOUT      0x05    /* Rx timeout    */

#define SERIAL_DMA_RX                0x01
#define SERIAL_DMA_TX                0x02

#define SERIAL_RX_INT                0x01
#define SERIAL_TX_INT                0x02

#define SERIAL_ERR_OVERRUN           0x01
#define SERIAL_ERR_FRAMING           0x02
#define SERIAL_ERR_PARITY            0x03

#define SERIAL_TX_DATAQUEUE_SIZE     2048
#define SERIAL_TX_DATAQUEUE_LWM      30

/* Default config for serial_configure structure */
#define SERIAL_CONFIG_DEFAULT           \
{                                          \
    BAUD_RATE_115200, /* 115200 bits/s */  \
    DATA_BITS_8,      /* 8 databits */     \
    STOP_BITS_1,      /* 1 stopbit */      \
    PARITY_NONE,      /* No parity  */     \
    BIT_ORDER_LSB,    /* LSB first sent */ \
    NRZ_NORMAL,       /* Normal mode */    \
    SERIAL_RB_BUFSZ, /* Buffer size */  \
    0                                      \
}

struct serial_configure
{
    e_uint32_t baud_rate;

    e_uint32_t data_bits               :4;
    e_uint32_t stop_bits               :2;
    e_uint32_t parity                  :2;
    e_uint32_t bit_order               :1;
    e_uint32_t invert                  :1;
    e_uint32_t bufsz                   :16;
    e_uint32_t reserved                :4;
};


struct serial_rx_fifo
{
    e_uint8_t *buffer;
    e_uint16_t put_index, get_index;
    e_size_t bufsz;
};
struct serial{
    e_uint16_t flag;
    e_uint16_t oflag;
    const struct serial_operation *ops;
    struct serial_configure config;
    
    void *serial_rx;
    void *serial_tx;
    
    os_mutex_t lock;
    
    void *private_data;
};
typedef struct serialice e_serial_t;

struct serial_operation{
    e_err_t (*init)(struct serial *dev);
    int (*fputc)(struct serial *dev, char c);
    int (*fgetc)(struct serial *dev);
};



e_err_t serial_register(int fd,
                    struct serial *serial,
                    const char *name);
struct serial * serial_open(int fd, int oflags);
void serial_close(struct serial *serial);
e_size_t serial_write(struct serial *serial,
                const void *buffer,
                e_size_t size);
e_size_t serial_read(struct serial *serial, 
                void *buffer,
                e_size_t size);
void serial_hw_isr(struct serial *serial, int event);
#endif