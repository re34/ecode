#include "sys_arch.h"
#include "types.h"
#include "time.h"

/**
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1
* @param  None
* @retval Time
*/
e_uint32_t sys_jiffies(void)
{
  //return HAL_GetTick();
  return system_get_time();
}

/**
* @brief  Returns the current time in milliseconds
*         when LWIP_TIMERS == 1 and NO_SYS == 1
* @param  None
* @retval Time
*/
e_uint32_t sys_now(void)
{
  //return HAL_GetTick();
  return system_get_time();
}