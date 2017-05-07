#include "mcu.h"
#include "ecode_tick.h"


void mcu_init()
{
    bsp_clock_config();
    ecode_tick_init();
}