#include "board.h"
#include "ecode.h"


extern void ecode_application_init(void);

void ecode_startup(void)
{
	ecode_hw_board_init();
	
	ecode_application_init();
}


void main(void)
{
	ecode_startup();

	rtos_start_scheduler();
}




