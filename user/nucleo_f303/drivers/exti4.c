#include "exti4.h"
#include "board_includes.h"

static exti4_callback_t _exti4_callback=NULL;


void exti4_init(exti4_callback_t callback)
{
    _exti4_callback = callback;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    GPIO_InitStructure.Pin = GPIO_PIN_4;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

void EXTI4_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_4)!=RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_4);
        if(_exti4_callback!=NULL)
            _exti4_callback();
    }    
}