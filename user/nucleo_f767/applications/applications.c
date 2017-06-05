#include "board.h"
#include "tcp_server.h"

static struct ecode_cli_dev com_cli;
static struct stdioex_device com_stdio;

void cli_task(void *args);
void led_task(void *args);

void ecode_application_init(void)
{
    tcp_server_init();
	xTaskCreate(cli_task,
            "cli_task",
            1024,
            NULL,
            1,
            NULL);
	xTaskCreate(led_task,
            "led_task",
            1024,
            NULL,
            3,
            NULL);	
}


static inline int com_putchar(unsigned char data)
{
    //write(COM1, (char *)&data, 1);
    serial_write(COM1, (char *)&data, 1);
    return 0;
}
static inline int com_getchar(void)
{
    int data;
    
    while((data=serial_in_waiting(COM1))<=0)
    {
        delay_ms(5);
    }
    //data = serial_in_waiting(COM1);
    //if(data<=0)
     //   return 0;
    //read(COM1, (char *)&data, 1);
    if(serial_read(COM1, (char *)&data, 1)<0)
        return 0;
    return data;
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

    while(1)
    {
        ecode_cli_polling();
    }
}

void led_task(void *args)
{
	stm_pin_init(LED1, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
	
	while(1)
	{
		stm_pin_toggle(LED1);
		delay_ms(500);
	}
}


