#include "board.h"
#include "uart.h"
#include "uart2.h"
#include "eth.h"
#include "pwm2.h"
#include "pwm3.h"
#include "user_oled.h"

void board_clock_configuration(void);

/**
 * This function will initial STM32 board.
 */
void ecode_hw_board_init()
{
    struct print_log_interface fprint_log;
    
	__set_PRIMASK(1);
	
    uart_init();
    
    fprint_log.put_char = uart_putc;
    
    print_log_register_io(fprint_log);
    
	uart2_init();
	
	pwm2_init();
	
	pwm3_init();
	
	user_oled_init();
	
    eth_init();
}

#if RTOS_EN==1
void vApplicationTickHook( void ){
	tick_inc();
    HAL_IncTick();
}
#if configCHECK_FOR_STACK_OVERFLOW==1
void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName){
	print_log("任务:%s发现栈溢出\r\n", pcTaskName);
}
#endif
#endif

/**
 * @addtogroup STM32
 */
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}
/*@{*/

/*******************************************************************************
* Function Name  : board_clock_configuration
* Description    : Configures board clock
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void board_clock_configuration(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL8;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

  //HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
