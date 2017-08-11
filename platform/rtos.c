#include "rtos.h"
#include "types.h"
#include "print_log.h"
#include "mem.h"





extern void xPortSysTickHandler( void );


static int in_isr_mode(void)
{
    return __get_IPSR()!=0;
}

static unsigned portBASE_TYPE os_make_priority(os_priority_t priority)
{
    unsigned portBASE_TYPE fpriority = tskIDLE_PRIORITY;
    
    if(priority!=OS_PRIO_ERROR){
        fpriority += (priority - OS_PRIO_IDLE);
    }
    return fpriority;
} 

e_err_t os_thread_new(os_thread_t *thread)
{

    if(thread==NULL)
        return -E_ERROR;
    
    if(xTaskCreate(thread->pthread, 
                    thread->name, 
                    thread->stacksz, 
                    thread->args, 
                    os_make_priority(thread->priority), 
                    &thread->handle)!=pdTRUE)
        return -E_ERROR;
        
    LOG_DEBUG("thread %s created!priority[%d]", thread->name, os_make_priority(thread->priority));
    
    return E_EOK;
}


os_sem_t os_sem_new(e_uint16_t count)
{
    os_sem_t sem;
    if(count==0)
    {
        sem = xSemaphoreCreateBinary();
    }
    else{
        sem = xSemaphoreCreateCounting(count, count);
    }
    return sem;
}

e_err_t os_sem_wait(os_sem_t sem, uint32_t millisec)
{
    TickType_t ticks;
    portBASE_TYPE task_woken = pdFALSE;
    
    if(!os_is_running())
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

e_err_t os_sem_post(os_sem_t sem)
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

os_mutex_t os_mutex_new(void)
{
    return xSemaphoreCreateMutex();
}

e_err_t os_mutex_lock(os_mutex_t mutex, uint32_t millisec)
{
    TickType_t ticks;
    portBASE_TYPE task_woken = pdFALSE;
    
    if(!os_is_running())
        return E_EOK;
    
    
    if(millisec==OS_WAIT_FOREVER)
        ticks = portMAX_DELAY;
    else if(millisec!=0){
        ticks = millisec/portTICK_PERIOD_MS;
        if(ticks == 0)
            ticks = 1;
    }
    
    if(in_isr_mode()){
        if(xSemaphoreTakeFromISR(mutex, &task_woken)!=pdTRUE){
            return -E_ERROR;
        }
        else
        {
            portEND_SWITCHING_ISR(task_woken);
        }
    }
    else if(xSemaphoreTake(mutex, ticks)!=pdTRUE)
    {
        return -E_ERROR;
    }
    return E_EOK;
}

e_err_t os_mutex_unlock(os_mutex_t mutex)
{
    portBASE_TYPE task_woken = pdFALSE;
    
    
    if(in_isr_mode()){
        if(xSemaphoreGiveFromISR(mutex, &task_woken)!= pdTRUE){
            return -E_ERROR;
        }
        portEND_SWITCHING_ISR(task_woken);
    }
    else{
        if(xSemaphoreGive(mutex)!=pdTRUE){
            return -E_ERROR;
        }
    }
    return E_EOK;
}

void os_mutex_delete(os_mutex_t mutex)
{
    vSemaphoreDelete(mutex);
}

os_queue_t os_queue_new(e_uint16_t length, e_uint16_t item_size)
{
    return xQueueCreate(length, item_size);
}

void os_queue_delete(os_queue_t queue)
{
    vQueueDelete(queue);
}

e_err_t os_queue_send(os_queue_t queue, void *item, e_uint32_t millisec)
{
    TickType_t ticks;
    portBASE_TYPE task_woken = pdFALSE;
    
    if(!os_is_running())
        return E_EOK;
    
    
    if(millisec==OS_WAIT_FOREVER)
        ticks = portMAX_DELAY;
    else if(millisec!=0){
        ticks = millisec/portTICK_PERIOD_MS;
        if(ticks == 0)
            ticks = 1;
    }
    
    if(in_isr_mode()){
        if(xQueueSendFromISR(queue, item, &task_woken)!=pdTRUE){
            return -E_ERROR;
        }
        else
        {
            portEND_SWITCHING_ISR(task_woken);
        }
    }
    else if(xQueueSend(queue, item, ticks)!=pdTRUE)
    {
        return -E_ERROR;
    }
    return E_EOK;
}


e_err_t os_queue_receive(os_queue_t queue, void *item, e_uint32_t millisec)
{
    TickType_t ticks;
    portBASE_TYPE task_woken = pdFALSE;
    
    if(!os_is_running())
        return E_EOK;
    
    
    if(millisec==OS_WAIT_FOREVER)
        ticks = portMAX_DELAY;
    else if(millisec!=0){
        ticks = millisec/portTICK_PERIOD_MS;
        if(ticks == 0)
            ticks = 1;
    }
    
    if(in_isr_mode()){
        if(xQueueReceiveFromISR(queue, item, &task_woken)!=pdTRUE){
            return -E_ERROR;
        }
        else
        {
            portEND_SWITCHING_ISR(task_woken);
        }
    }
    else if(xQueueReceive(queue, item, ticks)!=pdTRUE)
    {
        return -E_ERROR;
    }
    return E_EOK;
}

int os_is_running(void)
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

void os_kernel_start(void)
{
  LOG_DEBUG("os kernel starting...");
  vTaskStartScheduler();
}

void os_systick(void)
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

