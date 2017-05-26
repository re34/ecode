#include "mcu.h"
#include "ticks.h"


void mcu_init()
{
    bsp_clock_config();
   tick_init();
}