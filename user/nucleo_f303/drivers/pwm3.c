#include "pwm3.h"
#include "board_includes.h"
#include "ecode.h"

static struct pwm_device pwm3_dev;
static struct pwm_operations pwm3_ops;

#define PWM_TIMER_FREQ      16000000

int pwm3_hw_init(void);
void pwm3_period(unsigned int hz);
void pwm3_duty(float duty);
void pwm3_period_us(int us);
void pwm3_pulsewidth_us(int us);

int pwm3_init(void)
{
    pwm3_ops.init = pwm3_init;
    pwm3_ops.period = pwm3_period;
    pwm3_ops.pulseduty = pwm3_duty;
    pwm3_ops.period_us = pwm3_period_us;
    pwm3_ops.pulsewidth_us = pwm3_pulsewidth_us;
    
    pwm3_dev.ops = &pwm3_ops;
    
    if(pwm_register(PWM3, &pwm3_dev)<0)
    {
        LOG_ERROR("PWM3 register error!!!");
    }
	pwm_period_us(PWM3, 100);
	pwm_write(PWM3, 0);
	
	return 0;
}



int pwm3_hw_init(void)
{
  /*************************/
  /* GPIO AF configuration */
  /*************************/
  /* Enable the peripheral clock of GPIOs */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  
  /* GPIO TIM4_CH1 configuration */
  LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_DOWN);
  LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_6, LL_GPIO_AF_2);
  
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
  LL_TIM_SetPrescaler(TIM4, __LL_TIM_CALC_PSC(SystemCoreClock, PWM_TIMER_FREQ));
  
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

void pwm3_period(unsigned int hz)
{
    hz = PWM_TIMER_FREQ/hz;
    LL_TIM_SetAutoReload(TIM4, hz-1);
    LL_TIM_GenerateEvent_UPDATE(TIM4);
}

void pwm3_duty(float duty)
{
    unsigned int reload;
    
    reload = LL_TIM_GetAutoReload(TIM4);
    
    reload = (int)(reload*duty);
    if(reload!=0)
        reload -= 1;
    LL_TIM_OC_SetCompareCH1(TIM4, reload);
    LL_TIM_EnableCounter(TIM4);
}

void pwm3_period_us(int us)
{
    if(us>0xFFFF)
        us = 0xFFFF;
    LL_TIM_SetAutoReload(TIM4, us-1);
    LL_TIM_GenerateEvent_UPDATE(TIM4);
}

void pwm3_pulsewidth_us(int us)
{
    if(us > 0xFFFF)
        us = 0xFFFF;
    LL_TIM_OC_SetCompareCH1(TIM4, us);
    LL_TIM_EnableCounter(TIM4);
}
