#ifndef __TARGET_H__
#define __TARGET_H__
#include "config.h"
#include "bsp_clock.h"
#include "bsp_tick.h"
#include "bsp_mcu.h"
#include "bsp_uart.h"
#include "bsp_uart2.h"
#include "bsp_pin.h"
#include "bsp_base_timer.h"
#include "bsp_pwm.h"

#include "target_info.h"


#if RTOS_EN==1
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "event_groups.h"
#include "semphr.h"
#endif
#endif