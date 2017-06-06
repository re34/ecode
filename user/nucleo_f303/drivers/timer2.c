#include "timer2.h"
#include "board_includes.h"
#include "ecode.h"

#define TIMER1_MAX_COUNTER      0x7FFFFFFF

static struct timer_device timer2_dev;

static void timer2_hw_init(void);
static void timer2_start(void);
static void timer2_stop(void);
static int timer2_read_us(void);
static void timer2_reset(void);

int timer2_init(void)
{
    timer2_dev.start = timer2_start;
    timer2_dev.stop = timer2_stop;
    timer2_dev.read_us = timer2_read_us;
    timer2_dev.reset = timer2_reset;
    if(timer_register(TIMER2, &timer2_dev)<0)
        return -1;

    timer2_hw_init();
    
    return 0;
}

static void timer2_hw_init(void)
{
    int reload = 0;
    
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2); 
  

    LL_TIM_SetPrescaler(TIM2, __LL_TIM_CALC_PSC(SystemCoreClock, 1000000));
  
    /* Set the auto-reload value to have an initial update event frequency of 10 Hz */
    reload = __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM2), TIMER1_MAX_COUNTER);
    LL_TIM_SetAutoReload(TIM2, reload);
  

    /* Enable counter */
    LL_TIM_DisableCounter(TIM2);
    
    /* Enable the update interrupt */
    LL_TIM_EnableIT_UPDATE(TIM2);
      
    /* Configure the NVIC to handle TIM1 update interrupt */
    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);   
}

static void timer2_start(void)
{
    LL_TIM_EnableCounter(TIM2);
}

static void timer2_stop(void)
{
    LL_TIM_DisableCounter(TIM2);
}

static int timer2_read_us(void)
{
    return LL_TIM_GetCounter(TIM2);
}

static void timer2_reset(void)
{
    LL_TIM_SetCounter(TIM2,0);
}

void TIM2_IRQHandler(void)
{
    if(LL_TIM_IsActiveFlag_UPDATE(TIM2)==1)
    {
        LL_TIM_ClearFlag_UPDATE(TIM2);
    }
}