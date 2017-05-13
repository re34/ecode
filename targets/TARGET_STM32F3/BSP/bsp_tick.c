#include "bsp_tick.h"

void target_tick_init(uint32_t ticks_per_second, uint32_t priority)
{
  /*Configure the SysTick to have interrupt in 1ms time basis*/
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/ticks_per_second);

  /*Configure the SysTick IRQ priority */
  HAL_NVIC_SetPriority(SysTick_IRQn, priority ,0);
}


