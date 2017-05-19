#include "bsp_base_timer.h"

struct bsp_base_timer{
    uint16_t msecond;
    int second;
};

struct bsp_base_timer bsp_base_timer={
    .msecond = 0,
    .second = 0,
};

int bsp_base_timer_init(void)
{
    int reload = 0;
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM7); 
  

    LL_TIM_SetPrescaler(TIM7, __LL_TIM_CALC_PSC(SystemCoreClock, 1000000));
  
    /* Set the auto-reload value to have an initial update event frequency of 10 Hz */
    reload = __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM7), 1000);
    LL_TIM_SetAutoReload(TIM7, reload);
  

    /* Enable counter */
    LL_TIM_DisableCounter(TIM7);
    
    /* Enable the update interrupt */
    LL_TIM_EnableIT_UPDATE(TIM7);
      
    /* Configure the NVIC to handle TIM1 update interrupt */
    NVIC_SetPriority(TIM7_IRQn, 0);
    NVIC_EnableIRQ(TIM7_IRQn);      
  
    return 0;
}

void bsp_base_timer_start(void)
{
    LL_TIM_EnableCounter(TIM7);   
}

void bsp_base_timer_stop(void)
{
    LL_TIM_DisableCounter(TIM7);    
}

void bsp_base_timer_reset(void)
{
    LL_TIM_SetCounter(TIM7, 0);
    bsp_base_timer.msecond = 0;
    bsp_base_timer.second = 0;
}

float bsp_base_timer_read(void)
{
    float second;
    
    second = bsp_base_timer.second+bsp_base_timer.msecond*1.0f/1000+LL_TIM_GetCounter(TIM7)*1.0f/1000000;
    
    return second;
}

int bsp_base_timer_read_ms(void)
{
    int ms;
    
    ms = bsp_base_timer.msecond+bsp_base_timer.second*1000;
    
    return ms;
}

int bsp_base_timer_read_us(void)
{
    int us;
    
    us = bsp_base_timer.msecond*1000+bsp_base_timer.second*1000000+LL_TIM_GetCounter(TIM7);
    
    return us;
}

void TIM7_IRQHandler(void)
{
    if(LL_TIM_IsActiveFlag_UPDATE(TIM7)==1)
    {
        LL_TIM_ClearFlag_UPDATE(TIM7);
        bsp_base_timer.msecond ++;
        if(bsp_base_timer.msecond>=1000)
        {
            bsp_base_timer.second++;
            bsp_base_timer.msecond-=1000;
        }
    }
}


