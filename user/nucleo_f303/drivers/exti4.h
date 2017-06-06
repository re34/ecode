#ifndef __EXIT4_H__
#define __EXIT4_H__


typedef void (*exti4_callback_t)(void);

void exti4_init(exti4_callback_t callback);

#endif