#ifndef __HCSR04_H__
#define __HCSR04_H__
#include "ecode.h"

typedef void (*hcsr04_echo_callback_t)(void);

void hcsr04_hw_init(void);
void hcsr04_set_echo_callback(hcsr04_echo_callback_t callback);
void hcsr04_set_trig(pin_val_t status);
int hcsr04_get_echo(void);


#endif