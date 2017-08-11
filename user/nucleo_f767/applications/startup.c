#include "board.h"
#include "ecode.h"


extern void ecode_application_init(void);

void ecode_startup(void)
{
	ecode_hw_board_init();
	LOG_DEBUG("board inited!");
	ecode_application_init();
    LOG_DEBUG("application inited!");
}

void main(void)
{

	ecode_startup();

	os_kernel_start();
}




