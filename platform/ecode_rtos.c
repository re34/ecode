#include "ecode_rtos.h"
#include "ecode.h"

extern void xPortSysTickHandler( void );

rtos_task_run_t finit_run = NULL;
static int is_system_on = 0;

void wait_system_on(void)
{
    while(is_system_on==0)
    {
        vTaskDelay(1);
    }
}


void init_task(void *args)
{
    if(finit_run!=NULL)
        finit_run(NULL);
    
    while(1)
    {
        LOG_DEBUG("init task was removed!");
        is_system_on = 1;
        vTaskDelete(NULL);
        vTaskDelay(500);  
    }
}

void rtos_start(rtos_task_run_t run)
{
    finit_run = run;
    xTaskCreate(init_task,
                "init_task",
                1024,
                NULL,
                1,
                NULL);
                
    vTaskStartScheduler();
}

void rtos_start_scheduler(void)
{
  vTaskStartScheduler();
}

void rtos_systick(void)
{
   xPortSysTickHandler();
}

