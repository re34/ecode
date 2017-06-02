#include "board.h"
#include "ecode.h"


extern void ecode_application_init(void);

void ecode_startup(void *args)
{
	ecode_hw_board_init();
	ecode_application_init();
}


void main(void)
{

    
    board_clock_configuration();

    rtos_start(ecode_startup);
}




