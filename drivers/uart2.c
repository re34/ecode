#include "uart2.h"
#include "target.h"
//#include "serial.h"
#include "ecode.h"

#define SERIAL_RX_BUFFER_SIZE       100


static struct serial_dev serial;


static Fifo serial_rx_fifo;
static unsigned char serial_rx_buffer[SERIAL_RX_BUFFER_SIZE];

static int uart_putc(unsigned char c);
static int uart_getc(void);
static int uart_in_waiting(void);
static void uart_rx_irq(void);

int uart2_init(void)
{
    uart2_irq_t fuart_irq;
    serial.baudrate = 9600;
    
    if(FifoCreate(&serial_rx_fifo, serial_rx_buffer, SERIAL_RX_BUFFER_SIZE)<0)
        return -1;
    
    if(bsp_uart2_init(serial.baudrate)<0)
        return -2;
    serial.put_c = uart_putc;
    serial.get_c = uart_getc;
    serial.in_waiting = uart_in_waiting;
    if(serial_register(COM2, &serial)<0)
        return -3;
    fuart_irq.uart_rxne_callback = uart_rx_irq;
    bsp_uart2_register_irq(fuart_irq);
    return 0;
}

static int uart_putc(unsigned char c)
{
    bsp_uart2_writebyte(c);
    return 0;
}

static int uart_getc(void)
{
    while(FifoIsEmpty(&serial_rx_fifo));
    return FifoPop(&serial_rx_fifo);
}

static int uart_in_waiting(void)
{
    return FifoQueryPushed(&serial_rx_fifo);
}

static void uart_rx_irq(void)
{
    int data;
    data = bsp_uart2_getbyte();
    if(FifoIsFull(&serial_rx_fifo)!=1)
        FifoPush(&serial_rx_fifo, data);
}

