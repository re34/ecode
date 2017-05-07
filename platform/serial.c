#include "serial.h"
#include "includes.h"
#include "platform_driver.h"
#include "uart.h"


#define SERIAL_MINOR_NUM    2
#define SERIAL_RX_BUFFER_SIZE   128

static Fifo fserial_rx_fifo[SERIAL_MINOR_NUM];
static UInt8 fserial_rx_buffer[SERIAL_MINOR_NUM][SERIAL_RX_BUFFER_SIZE];

static Serial_t fserials[SERIAL_MINOR_NUM];

static int serial_write(struct device_file *file, char *datas, int len);
static int serial_read(struct device_file *file, char *buf, int len);
static int serial_ioctl(struct device_file *file, int cmd, int arg);

static struct device_operations ops={
    .write = serial_write,
    .read = serial_read,
    .ioctl = serial_ioctl,
};


static struct device_file file[2];

static struct platform_device com_dev={
    .name = "COM",
    .file = file,
    .ops = &ops,
};


void serial1_rx_callback(void)
{
    int data;
    data = bsp_uart_getbyte();
    if(FifoIsFull(&fserial_rx_fifo[0])!=1)
        FifoPush(&fserial_rx_fifo[0], data);
}


int serial_init(void)
{
    int ret;
    struct serial_operations serial_ops;
    uart_irq_t fuart_irq;
    struct print_log_interface fprint_log;

    
    if(FifoCreate(&fserial_rx_fifo[0],fserial_rx_buffer[0], SERIAL_RX_BUFFER_SIZE)<0)
        return -1;
    
    serial_ops.get_byte = bsp_uart_getbyte;
    serial_ops.write_byte = bsp_uart_writebyte;
    fserials[0].ops=serial_ops;
    fserials[0].baudrate = 115200;
    file[0].content = &fserials[0];
    
    bsp_uart_init(fserials[0].baudrate);
    fuart_irq.uart_rxne_callback = serial1_rx_callback;
    bsp_uart_register_irq(fuart_irq);
    
    fprint_log.putchar = bsp_uart_writebyte;
    print_log_register_io(fprint_log);
    
    ret = platform_driver_register(COM, 2,&com_dev);
    if(ret<0)
        return -2;
    
    LOG_DEBUG("serial inited!");
    return 0;
}

static int serial_write(struct device_file *file, char *datas, int len)
{
    int ret;
    Serial_t *fserial = &fserials[file->minor];
    struct serial_operations *fops = &fserial->ops;
    char *pdata = datas;
    while(len--){
        fops->write_byte(*pdata++);
        ret++;
    }
    
    return ret;
}


static int serial_pop_byte(int fd)
{
    while(FifoIsEmpty(&fserial_rx_fifo[fd]));
    return FifoPop(&fserial_rx_fifo[fd]);
}

static int serial_read_byte(struct device_file *file)
{
    return serial_pop_byte(file->minor);
}

static int serial_read(struct device_file *file, char *buf, int len)
{
    int ret=0;
    Serial_t *fserial = &fserials[file->minor];
    struct serial_operations *fops = &fserial->ops;
   
    while(len--){
        *buf++=serial_pop_byte(file->minor);
        ret++;
    }
    
    return ret;
}

static int serial_ioctl(struct device_file *file, int cmd, int arg)
{
    switch(cmd)
    {
    case 0:
        break;
    case 1:
        break;
    }
    
    return 0;
}



