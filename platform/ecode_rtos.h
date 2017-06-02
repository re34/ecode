#ifndef __ECODE_RTOS_H__
#define __ECODE_RTOS_H__


int rtos_is_running(void);
void rtos_start_scheduler(void);
void rtos_systick(void);
#endif