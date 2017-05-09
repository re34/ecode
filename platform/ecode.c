#include "ecode.h"
#include "target.h"
#include "platform_cli_handles.h"
#include <stdio.h>


static struct ecode_cli_dev com_cli;
static struct stdioex_device com_stdio;
static struct timer_device base_timer;
static struct pwm_device pwm_dev;
static struct pwm_operations pwm_ops;



static inline int com_putchar(unsigned char data)
{
    write(COM1, (char *)&data, 1);
    return 0;
}
static inline int com_getchar(void)
{
    int data;
    read(COM1, (char *)&data, 1);
    return data;
}

void ecode_init(void)
{
    struct timestamp timestamp;
    
    ecode_tick_set_callback(timestamp_polling);
    serial_init();
    
    com_stdio.put_char = com_putchar;
    com_stdio.get_char = com_getchar;
    stdio_puts(&com_stdio, "ecode initing......\r\n");
    printf("ecode initing...\r\n");
    com_cli.stdio = &com_stdio;
    ecode_register_cli_device( &com_cli, "COM");
    cli_register_platform_commands();
    
    //register base timer
    base_timer.init = bsp_base_timer_init;
    base_timer.start = bsp_base_timer_start;
    base_timer.stop = bsp_base_timer_stop;
    base_timer.reset = bsp_base_timer_reset;
    base_timer.read = bsp_base_timer_read;
    base_timer.read_ms = bsp_base_timer_read_ms;
    base_timer.read_us = bsp_base_timer_read_us;
    if(timer_register(TIMER1, &base_timer)<0)
    {
        LOG_ERROR("TIMER1 register error!!!");
    }
    
    pwm_ops.init = bsp_pwm_init;
    pwm_ops.period_us = bsp_pwm_period_us;
    pwm_ops.pulsewidth_us = bsp_pwm_pulsewidth_us;
    
    pwm_dev.ops = &pwm_ops;
    
    if(pwm_register(PWM1, &pwm_dev)<0)
    {
        LOG_ERROR("PWM1 register error!!!");
    }
    
    
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