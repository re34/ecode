#include "board.h"
#include "ecode.h"
#include "command_service.h"
#include "led_task.h"



void ecode_application_init(void)
{
    cli_init();
    command_service_init();
    led_task_init();
    //tcp_server_init();
    
}


<<<<<<< HEAD
void cli_task(void *args)
{
    cli_register_platform_commands();
    
    com_stdio.put_char = com_putchar;
    com_stdio.get_char = com_getchar;
    stdio_puts(&com_stdio, "ecode stdio inited\r\n");
    com_cli.stdio = &com_stdio;
    cli_device_register( &com_cli, "COM");
    
    LOG_DEBUG("cli task running...");

    while(1)
    {
        if(cli_polling()<0)
            delay_ms(5);
    }
}

void led_task(void *args)
{
    pin_val_t pin_val=PIN_LOW;
	//stm_pin_init(LED1, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);
    pin_mode(LED1, PIN_MODE_OUTPUT_PP, PIN_PUPD_UP);

	while(1)
	{
		//stm_pin_toggle(LED1);
        pin_write(LED1, pin_val);
        pin_val = !pin_val;
		delay_ms(500);
	}
}

=======
>>>>>>> master

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
