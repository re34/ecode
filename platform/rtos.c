#include "rtos.h"
#include "types.h"
#include "print_log.h"





extern void xPortSysTickHandler( void );


static int in_isr_mode(void)
{
    return __get_IPSR()!=0;
}



void rtos_task_create(    TaskFunction_t task,
                            const char * const name,
                            const uint16_t stack_depth,
                            void * const args,
                            UBaseType_t priority,
                            TaskHandle_t * const handle )
{
    xTaskCreate(task, name, stack_depth, args, priority, handle);
    LOG_DEBUG("task %s created!", name);
}

void os_task_create(TaskFunction_t task,
                    const char *name,
                    const uint16_t stack_depth,
                    os_priority_t prio,
                    TaskHandle_t *const handle)
{
    
    
}

os_sem_t os_sem_create(void)
{
    return xSemaphoreCreateBinary();
}

e_err_t os_sem_wait(os_sem_t sem, uint32_t millisec)
{
    TickType_t ticks;
    portBASE_TYPE task_woken = pdFALSE;
    
    if(!rtos_is_running())
        return E_EOK;
    
    
    if(millisec==OS_WAIT_FOREVER)
        ticks = portMAX_DELAY;
    else if(millisec!=0){
        ticks = millisec/portTICK_PERIOD_MS;
        if(ticks == 0)
            ticks = 1;
    }
    
    if(in_isr_mode()){
        if(xSemaphoreTakeFromISR(sem, &task_woken)!=pdTRUE){
            return -E_ERROR;
        }
        else
        {
            portEND_SWITCHING_ISR(task_woken);
        }
    }
    else if(xSemaphoreTake(sem, ticks)!=pdTRUE)
    {
        return -E_ERROR;
    }
    return E_EOK;
}

e_err_t os_sem_release(os_sem_t sem)
{
    portBASE_TYPE task_woken = pdFALSE;
    
    
    if(in_isr_mode()){
        if(xSemaphoreGiveFromISR(sem, &task_woken)!= pdTRUE){
            return -E_ERROR;
        }
        portEND_SWITCHING_ISR(task_woken);
    }
    else{
        if(xSemaphoreGive(sem)!=pdTRUE){
            return -E_ERROR;
        }
    }
    return E_EOK;
}

void os_sem_delete(os_sem_t sem)
{
    vSemaphoreDelete(sem);
}

int rtos_is_running(void)
{
#if ( ( INCLUDE_xTaskGetSchedulerState == 1 ) || ( configUSE_TIMERS == 1 ) )
  if (xTaskGetSchedulerState() == taskSCHEDULER_NOT_STARTED)
    return 0;
  else
    return 1;
#else
    return (-1);
#endif      
}

void rtos_start_scheduler(void)
{
  vTaskStartScheduler();
}

void rtos_systick(void)
{
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
#endif  /* INCLUDE_xTaskGetSchedulerState */  
    xPortSysTickHandler();
#if (INCLUDE_xTaskGetSchedulerState  == 1 )
  }
#endif  /* INCLUDE_xTaskGetSchedulerState */  
}

