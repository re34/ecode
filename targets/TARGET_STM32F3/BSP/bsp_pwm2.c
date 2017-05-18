#include "bsp_pwm1.h"
#include "target_chip.h"


int bsp_pwm2_init(void)
{
  /*************************/
  /* GPIO AF configuration */
  /*************************/
  /* Enable the peripheral clock of GPIOs */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOC);
  
  /* GPIO TIM4_CH1 configuration */
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_6, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_6, LL_GPIO_AF_2);
  
  /******************************/
  /* Peripheral clocks enabling */
  /******************************/
  /* Enable the timer peripheral clock */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4); 
  
  /***************************/
  /* Time base configuration */
  /***************************/
  /* Set counter mode */
  /* Reset value is LL_TIM_COUNTERMODE_UP */
  LL_TIM_SetCounterMode(TIM4, LL_TIM_COUNTERMODE_UP);
  
  /* Set the pre-scaler value to have TIM4 counter clock equal to 1000 kHz */
  LL_TIM_SetPrescaler(TIM4, __LL_TIM_CALC_PSC(SystemCoreClock, 1000000));
  
  /* Enable TIM4_ARR register preload. Writing to or reading from the         */
  /* auto-reload register accesses the preload register. The content of the   */
  /* preload register are transferred into the shadow register at each update */
  /* event (UEV).                                                             */  
  LL_TIM_EnableARRPreload(TIM4);
  
  /* Set the auto-reload value to have a counter frequency of 100 Hz */
  LL_TIM_SetAutoReload(TIM4, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_GetPrescaler(TIM4), 100));
  
  /*********************************/
  /* Output waveform configuration */
  /*********************************/
  /* Set output mode */
  /* Reset value is LL_TIM_OCMODE_FROZEN */
  LL_TIM_OC_SetMode(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
  
  /* Set output channel polarity */
  /* Reset value is LL_TIM_OCPOLARITY_HIGH */
  LL_TIM_OC_SetPolarity(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH);
  
  /* Set compare value to half of the counter period (50% duty cycle ) */
  LL_TIM_OC_SetCompareCH1(TIM4, 0);
  
  /* Enable TIM4_CCR1 register preload. Read/Write operations access the      */
  /* preload register. TIM4_CCR1 preload value is loaded in the active        */
  /* at each update event.                                                    */
  LL_TIM_OC_EnablePreload(TIM4, LL_TIM_CHANNEL_CH1);
  
  /**********************************/
  /* Start output signal generation */
  /**********************************/
  /* Enable output channel 1 */
  LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
  
  /* Enable counter */
  //LL_TIM_DisableCounter(TIM4); 
  LL_TIM_EnableCounter(TIM4);
  return 0;
}


void bsp_pwm2_period_us(int us)
{
    if(us>0xFFFF)
        us = 0xFFFF;
    LL_TIM_SetAutoReload(TIM4, us-1);
    LL_TIM_GenerateEvent_UPDATE(TIM4);
}

void bsp_pwm2_pulsewidth_us(int us)
{
    if(us > 0xFFFF)
        us = 0xFFFF;
    LL_TIM_OC_SetCompareCH1(TIM4, us);
    LL_TIM_EnableCounter(TIM4);
}

