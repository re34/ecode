#include "ecode_rtos.h"
#include "includes.h"

rtos_task_run_t finit_run = NULL;

void init_task(void *args)
{
    if(finit_run!=NULL)
        finit_run(NULL);
    
    while(1)
    {
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

