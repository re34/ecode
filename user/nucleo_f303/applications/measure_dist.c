#include "measure_dist.h"
#include "board.h"

static void measure_dist_callback(void);

void measure_start(void)
{
	hcsr04_set_echo_callback(measure_dist_callback);
	hcsr04_set_trig(PIN_SET);
	delay_ms(1);
	hcsr04_set_trig(PIN_RESET);
	
}

int measure_get_dist(void)
{

  return 0;
}

static void measure_dist_callback(void)
{
	
	
	
}