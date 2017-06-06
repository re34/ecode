#include "timer1.h"
#include "board_includes.h"
#include "ecode.h"

#define TIMER1_MAX_COUNTER      0xFFFF

static int _us = 0;
static struct timer_device timer1_dev;

static void timer1_hw_init(void);
static void timer1_start(void);
static void timer1_stop(void);
static int timer1_read_us(void);
static void timer1_reset(void);

int timer1_init(void)
{
    timer1_dev.start = timer1_start;
    timer1_dev.stop = timer1_stop;
    timer1_dev.read_us = timer1_read_us;
    timer1_dev.reset = timer1_reset;
    if(timer_register(TIMER1, &timer1_dev)<0)
        return -1;
    _us = 0;
    timer1_hw_init();
    
    return 0;
}

static void timer1_hw_init(void)
{
    int reload = 0;
    
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7); 
  

    LL_TIM_SetPrescaler(TIM7, __LL_TIM_CALC_PSC(SystemCoreClock, 1000000));
  
    /* Set the auto-reload value to have an initial update event frequency of 10 Hz */
    reload = __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM7), TIMER1_MAX_COUNTER);
    LL_TIM_SetAutoReload(TIM7, reload);
  

    /* Enable counter */
    LL_TIM_DisableCounter(TIM7);
    
    /* Enable the update interrupt */
    LL_TIM_EnableIT_UPDATE(TIM7);
      
    /* Configure the NVIC to handle TIM1 update interrupt */
    NVIC_SetPriority(TIM7_IRQn, 0);
    NVIC_EnableIRQ(TIM7_IRQn);   
}

static void timer1_start(void)
{
    LL_TIM_EnableCounter(TIM7);
}

static void timer1_stop(void)
{
    LL_TIM_DisableCounter(TIM7);
}

static int timer1_read_us(void)
{
    return _us+LL_TIM_GetCounter(TIM7);
}

static void timer1_reset(void)
{
    LL_TIM_SetCounter(TIM7,0);
}

void TIM7_IRQHandler(void)
{
    if(LL_TIM_IsActiveFlag_UPDATE(TIM7)==1)
    {
        LL_TIM_ClearFlag_UPDATE(TIM7);
        _us+=TIMER1_MAX_COUNTER;
    }
}