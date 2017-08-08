#ifndef __RTOS_H__
#define __RTOS_H__
#include "config.h"
#include "types.h"

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


typedef SemaphoreHandle_t os_sem_t;

typedef QueueHandle_t os_queue_t;

typedef SemaphoreHandle_t os_mutex_t;


typedef enum{
    OS_PRIO_IDLE        = -3,
    OS_PRIO_LOW         = -2,
    OS_PRIO_BELOW_NORMAL    =-1,
    OS_PRIO_NORMAL      = 0,
    OS_PRIO_ABOVE_NORMAL = +1,
    OS_PRIO_HIGH        = +2,
    OS_PRIO_REAL_TIME   = +3,
    OS_PRIO_ERROR       = 0x84,
}os_priority_t;

#define OS_WAIT_FOREVER     0xFFFFFFFF
//#define rtos_task_create(task, name, stack_depth, args, priority, handle)  \
//                xTaskCreate(task, name, stack_depth, args, priority, handle)
void rtos_task_create(	TaskFunction_t task,
							const char * const name,
							const uint16_t stack_depth,
							void * const args,
							UBaseType_t priority,
							TaskHandle_t * const handle );
                            
void os_task_create(TaskFunction_t task,
                    const char *name,
                    const uint16_t stack_depth,
                    os_priority_t prio,
                    TaskHandle_t *const handle);
os_sem_t os_sem_create(void);
e_err_t os_sem_wait(os_sem_t sem, uint32_t millisec);
e_err_t os_sem_release(os_sem_t sem);
void os_sem_delete(os_sem_t sem);
os_mutex_t os_mutex_create(void);
e_err_t os_mutex_lock(os_mutex_t mutex, uint32_t millisec);
e_err_t os_mutex_unlock(os_mutex_t mutex);
void os_mutex_delete(os_mutex_t mutex);
int rtos_is_running(void);
void rtos_start_scheduler(void);
void rtos_systick(void);
#endif