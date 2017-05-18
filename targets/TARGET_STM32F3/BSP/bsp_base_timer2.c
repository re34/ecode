#include "bsp_base_timer2.h"

struct bsp_base_timer2{
    uint16_t msecond;
    int second;
};

struct bsp_base_timer2 bsp_base_timer2={
    .msecond = 0,
    .second = 0,
};

int bsp_base_timer2_init(void)
{
    int reload = 0;
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2); 
  

    LL_TIM_SetPrescaler(TIM2, __LL_TIM_CALC_PSC(SystemCoreClock, 1000000));
  
    /* Set the auto-reload value to have an initial update event frequency of 10 Hz */
    reload = __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM2), 1000);
    LL_TIM_SetAutoReload(TIM2, reload);
  

    /* Enable counter */
    LL_TIM_DisableCounter(TIM2);
    
    /* Enable the update interrupt */
    LL_TIM_EnableIT_UPDATE(TIM2);
      
    /* Configure the NVIC to handle TIM1 update interrupt */
    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);      
  
    return 0;
}

void bsp_base_timer2_start(void)
{
    LL_TIM_EnableCounter(TIM2);   
}

void bsp_base_timer2_stop(void)
{
    LL_TIM_DisableCounter(TIM2);    
}

void bsp_base_timer2_reset(void)
{
    LL_TIM_SetCounter(TIM2, 0);
    bsp_base_timer2.msecond = 0;
    bsp_base_timer2.second = 0;
}

float bsp_base_timer2_read(void)
{
    float second;
    
    second = bsp_base_timer2.second+bsp_base_timer2.msecond*1.0f/1000+LL_TIM_GetCounter(TIM2)*1.0f/1000000;
    
    return second;
}

int bsp_base_timer2_read_ms(void)
{
    int ms;
    
    ms = bsp_base_timer2.msecond+bsp_base_timer2.second*1000;
    
    return ms;
}

int bsp_base_timer2_read_us(void)
{
    int us;
    
    us = bsp_base_timer2.msecond*1000+bsp_base_timer2.second*1000000+LL_TIM_GetCounter(TIM2);
    
    return us;
}

void TIM2_IRQHandler(void)
{
    if(LL_TIM_IsActiveFlag_UPDATE(TIM2)==1)
    {
        LL_TIM_ClearFlag_UPDATE(TIM2);
        bsp_base_timer2.msecond ++;
        if(bsp_base_timer2.msecond>=1000)
        {
            bsp_base_timer2.second++;
            bsp_base_timer2.msecond-=1000;
        }
    }
}


