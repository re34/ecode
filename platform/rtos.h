#ifndef __RTOS_H__
#define __RTOS_H__
#include "config.h"

#if RTOS_EN==1
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "croutine.h"
#include "event_groups.h"
#include "semphr.h"
#endif



//#define rtos_task_create(task, name, stack_depth, args, priority, handle)  \
//                xTaskCreate(task, name, stack_depth, args, priority, handle)
void rtos_task_create(	TaskFunction_t task,
							const char * const name,
							const uint16_t stack_depth,
							void * const args,
							UBaseType_t priority,
							TaskHandle_t * const handle );

int rtos_is_running(void);
void rtos_start_scheduler(void);
void rtos_systick(void);
#endif