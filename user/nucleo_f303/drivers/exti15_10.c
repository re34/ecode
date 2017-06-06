#include "exti15_10.h"
#include "board_includes.h"

static exti15_10_callback_t _exti15_10_callback=NULL;


void exti15_10_init(exti15_10_callback_t callback)
{
    _exti15_10_callback = callback;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    GPIO_InitStructure.Pin = GPIO_PIN_13;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
}

void EXTI15_10_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_13)!=RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);
        if(_exti15_10_callback!=NULL)
            _exti15_10_callback();
    }    
}