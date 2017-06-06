#include "hcsr04.h"
#include "board_includes.h"
#include "ecode.h"

static hcsr04_echo_callback_t _hcsr04_echo_callback = NULL;

void hcsr04_hw_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    GPIO_InitStructure.Pin = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLDOWN;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);  
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
    GPIO_InitStructure.Pin = GPIO_PIN_3;
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING_FALLING;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI3_IRQn);
    
    LOG_DEBUG("hcsr04 is inited!");
}

void hcsr04_set_echo_callback(hcsr04_echo_callback_t callback)
{
	_hcsr04_echo_callback = callback;
}

void hcsr04_set_trig(pin_state_t status)
{
	if(status==PIN_SET)
	{
        HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
    }
}

int hcsr04_get_echo(void)
{
    if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)==GPIO_PIN_SET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


void EXTI3_IRQHandler(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(GPIO_PIN_3)!=RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_3);
        if(_hcsr04_echo_callback!=NULL)
            _hcsr04_echo_callback();
    }
}
