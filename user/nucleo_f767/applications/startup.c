#include "board.h"
#include "ecode.h"

void init(void *args);

extern void ecode_application_init(void);

void ecode_startup(void)
{
	ecode_hw_board_init();
	
}

void main(void)
{
    board_clock_configuration();
    
    rtos_start(init);
}

static struct ecode_cli_dev com_cli;
static struct stdioex_device com_stdio;


static inline int com_putchar(unsigned char data)
{
    //write(COM1, (char *)&data, 1);
    serial_write(COM1, (char *)&data, 1);
    return 0;
}
static inline int com_getchar(void)
{
    int data;
    
    data = serial_in_waiting(COM1);
    if(data<=0)
        return 0;
    //read(COM1, (char *)&data, 1);
    if(serial_read(COM1, (char *)&data, 1)<0)
        return 0;
    return data;
}

void cli_task(void *args);
void init(void *args)
{
    ecode_startup();
    xTaskCreate(cli_task,
                "cli_task",
                1024,
                NULL,
                2,
                NULL);
}




void cli_task(void *args)
{
    cli_register_platform_commands();
    
    com_stdio.put_char = com_putchar;
    com_stdio.get_char = com_getchar;
    stdio_puts(&com_stdio, "ecode stdio inited\r\n");
    com_cli.stdio = &com_stdio;
    ecode_register_cli_device( &com_cli, "COM");
    
    LOG_DEBUG("cli task running...");
    wait_system_on();
    while(1)
    {
        ecode_cli_polling();
    }
}