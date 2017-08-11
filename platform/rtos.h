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


typedef SemaphoreHandle_t os_sem_t;

typedef QueueHandle_t os_queue_t;

typedef SemaphoreHandle_t os_mutex_t;

typedef QueueHandle_t os_mbox_t;

typedef TaskHandle_t os_thread_handle;

typedef TaskFunction_t os_pthread;

//typedef void (*os_pthread)(void const *args);

typedef struct{
    char *name;
    os_pthread pthread;
    e_uint16_t stacksz;
    os_priority_t priority;
    os_thread_handle handle;
    void *args;
}os_thread_t;


#define OS_WAIT_FOREVER     0xFFFFFFFF

e_err_t os_thread_new(os_thread_t *thread);
os_sem_t os_sem_new(e_uint16_t count);
e_err_t os_sem_wait(os_sem_t sem, uint32_t millisec);
e_err_t os_sem_post(os_sem_t sem);
void os_sem_delete(os_sem_t sem);
os_mutex_t os_mutex_new(void);
e_err_t os_mutex_lock(os_mutex_t mutex, uint32_t millisec);
e_err_t os_mutex_unlock(os_mutex_t mutex);
void os_mutex_delete(os_mutex_t mutex);
os_queue_t os_queue_new(e_uint16_t length, e_uint16_t item_size);
void os_queue_delete(os_queue_t queue);
e_err_t os_queue_send(os_queue_t queue, void *item, e_uint32_t millisec);
e_err_t os_queue_receive(os_queue_t queue, void *item, e_uint32_t millisec);
int os_is_running(void);
void os_kernel_start(void);
void os_systick(void);
#endif