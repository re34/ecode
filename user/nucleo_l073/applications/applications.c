#include "board.h"
static struct ecode_cli_dev com_cli;
static struct stdioex_device com_stdio;

void cli_task(void *args);


void ecode_application_init(void)
{
	xTaskCreate(cli_task,
            "cli_task",
            1024,
            NULL,
            2,
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
    
    data = serial_in_waiting(COM1);
    if(data<=0)
        return 0;
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
    wait_system_on();
    while(1)
    {
        ecode_cli_polling();
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

