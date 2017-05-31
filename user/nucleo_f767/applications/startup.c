#include "board.h"
#include "ecode.h"


extern void ecode_application_init(void);

void ecode_startup(void)
{
	ecode_hw_board_init();
	
}

void main(void)
{
	ecode_startup();
}

#if RTOS_EN==1
void vApplicationTickHook( void ){
	tick_inc();
}
#if configCHECK_FOR_STACK_OVERFLOW==1
void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName){
	print_log("任务:%s发现栈溢出\r\n", pcTaskName);
}
#endif
#endif