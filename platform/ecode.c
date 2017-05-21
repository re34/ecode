#include "ecode.h"
#include "target.h"
#include "platform_cli_handles.h"
#include <stdio.h>
#include "driver.h"


static struct ecode_cli_dev com_cli;
static struct stdioex_device com_stdio;


static inline int com_putchar(unsigned char data)
{
    //write(COM1, (char *)&data, 1);
    serial_write(COM1, &data, 1);
    return 0;
}
static inline int com_getchar(void)
{
    int data;
    //read(COM1, (char *)&data, 1);
    serial_read(COM1, &data, 1);
    return data;
}

void ecode_init(void)
{
    struct timestamp timestamp;
    
    ecode_tick_set_callback(timestamp_polling);
    driver_init();
    
    com_stdio.put_char = com_putchar;
    com_stdio.get_char = com_getchar;
    stdio_puts(&com_stdio, "ecode stdio inited\r\n");
    com_cli.stdio = &com_stdio;
    ecode_register_cli_device( &com_cli, "COM");
    cli_register_platform_commands();
    
    timestamp = get_timestamp();
    LOG_INFO("start up time: %d s %d ms", timestamp.second, timestamp.msecond);
    
    FirmwareInfoPrint();
}



#if RTOS_EN==1
void vApplicationTickHook( void ){
	ecode_tick_inc();
}
#if configCHECK_FOR_STACK_OVERFLOW==1
void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName){
	print_log("任务:%s发现栈溢出\r\n", pcTaskName);
}
#endif
#endif

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