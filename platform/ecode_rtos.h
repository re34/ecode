#ifndef __ECODE_RTOS_H__
#define __ECODE_RTOS_H__

typedef void(*rtos_task_run_t)(void *args);


void rtos_start(rtos_task_run_t run);

#endif