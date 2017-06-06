#ifndef __EXTI15_10_H__
#define __EXTI15_10_H__


typedef void (*exti15_10_callback_t)(void);

void exti15_10_init(exti15_10_callback_t callback);
#endif