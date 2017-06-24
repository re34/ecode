#include "command_service.h"
#include "ecode.h"
#include "commands.h"


#define COMMAND_RX_BUFFER_SIZE      512

e_uint8_t command_rx_buffer[COMMAND_RX_BUFFER_SIZE];


int command_getc(void)
{
    int data;
    if(serial_read(COM1, &data, 1)>0)
        return data;
    else
        return -1;
}

int command_putc(char c)
{
    return serial_write(COM1, &c, 1);
}


const struct command_operations command_ops = {
    .getc = command_getc,
    .putc = command_putc,
};

struct command_dev command_service={
    .rx_buffer.buffer = command_rx_buffer,
    .rx_buffer.size = COMMAND_RX_BUFFER_SIZE,
    .ops = &command_ops,
};


void command_service_init(void)
{
    command_register_platform_commands();
    if(command_register(&command_service)!=E_EOK)
    {
        LOG_DEBUG("command service register failed!");
    }
    
}