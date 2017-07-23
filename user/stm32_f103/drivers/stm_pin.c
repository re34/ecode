#include "stm_pin.h"
#include "board_includes.h"
#include "ecode.h"


#define STM_PIN(pin)    ((pin)&0xF)  
#define STM_PORT(pin)   (((pin)>>4)&0x0F)


const uint32_t pin_defines[16] = {
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15
};

static inline GPIO_TypeDef *stm_pin_get_port(uint32_t port_idx)
{
    uint32_t gpio_base;
    
    switch (port_idx) {
        case PortA:
            gpio_base = GPIOA_BASE;
            break;
        case PortB:
            gpio_base = GPIOB_BASE;
            break;
#if defined(GPIOC_BASE)
        case PortC:
            gpio_base = GPIOC_BASE;
            break;
#endif
#if defined GPIOD_BASE
        case PortD:
            gpio_base = GPIOD_BASE;
            break;
#endif
#if defined GPIOE_BASE
        case PortE:
            gpio_base = GPIOE_BASE;
            break;
#endif
#if defined GPIOF_BASE
        case PortF:
            gpio_base = GPIOF_BASE;
            break;
#endif
#if defined GPIOG_BASE
        case PortG:
            gpio_base = GPIOG_BASE;
            break;
#endif
#if defined GPIOH_BASE
        case PortH:
            gpio_base = GPIOH_BASE;
            break;
#endif
#if defined GPIOI_BASE
        case PortI:
            gpio_base = GPIOI_BASE;
            break;
#endif
#if defined GPIOJ_BASE
        case PortJ:
            gpio_base = GPIOJ_BASE;
            break;
#endif
#if defined GPIOK_BASE
        case PortK:
            gpio_base = GPIOK_BASE;
            break;
#endif
    }
    
    return (GPIO_TypeDef *)gpio_base;
}


GPIO_TypeDef *stm_gpio_set_clk(uint32_t port_idx) {
    uint32_t gpio_add = 0;
    switch (port_idx) {
        case PortA:
            gpio_add = GPIOA_BASE;
            __HAL_RCC_GPIOA_CLK_ENABLE();
            break;
        case PortB:
            gpio_add = GPIOB_BASE;
            __HAL_RCC_GPIOB_CLK_ENABLE();
            break;
#if defined(GPIOC_BASE)
        case PortC:
            gpio_add = GPIOC_BASE;
            __HAL_RCC_GPIOC_CLK_ENABLE();
            break;
#endif
#if defined GPIOD_BASE
        case PortD:
            gpio_add = GPIOD_BASE;
            __HAL_RCC_GPIOD_CLK_ENABLE();
            break;
#endif
#if defined GPIOE_BASE
        case PortE:
            gpio_add = GPIOE_BASE;
            __HAL_RCC_GPIOE_CLK_ENABLE();
            break;
#endif
#if defined GPIOF_BASE
        case PortF:
            gpio_add = GPIOF_BASE;
            __HAL_RCC_GPIOF_CLK_ENABLE();
            break;
#endif
#if defined GPIOG_BASE
        case PortG:
            gpio_add = GPIOG_BASE;
            __HAL_RCC_GPIOG_CLK_ENABLE();
            break;
#endif
#if defined GPIOH_BASE
        case PortH:
            gpio_add = GPIOH_BASE;
            __HAL_RCC_GPIOH_CLK_ENABLE();
            break;
#endif
#if defined GPIOI_BASE
        case PortI:
            gpio_add = GPIOI_BASE;
            __HAL_RCC_GPIOI_CLK_ENABLE();
            break;
#endif
#if defined GPIOJ_BASE
        case PortJ:
            gpio_add = GPIOJ_BASE;
            __HAL_RCC_GPIOJ_CLK_ENABLE();
            break;
#endif
#if defined GPIOK_BASE
        case PortK:
            gpio_add = GPIOK_BASE;
            __HAL_RCC_GPIOK_CLK_ENABLE();
            break;
#endif
        default:
            printf("Pinmap error: wrong port number.");
            break;
    }
    return (GPIO_TypeDef *) gpio_add;
}


void stm_pin_mode(struct pin_device *dev, pin_index_t pin, pin_mode_t mode, pin_pupd_t pupd)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
    GPIO_TypeDef *port;
    uint32_t pin_mask = pin_defines[STM_PIN(pin)];
    
    port = stm_gpio_set_clk(STM_PORT(pin));
    
    GPIO_InitStruct.Pin = pin_mask;
    
	if(mode==PIN_MODE_INPUT)
	{
        GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
	}
	else if(mode==PIN_MODE_OUTPUT_PP)
	{
		GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
	}
	else if(mode==PIN_MODE_OUTPUT_OD)
	{
		GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_OD;
	}
	else if(mode==PIN_MODE_ANALOG)
	{
		GPIO_InitStruct.Mode      = GPIO_MODE_ANALOG;
	}
    
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    
    if(pupd==PIN_PUPD_UP)
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
    else if(pupd==PIN_PUPD_DOWN)
        GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
	else if(pupd==PIN_PUPD_NO)
		GPIO_InitStruct.Pull      = GPIO_NOPULL;
    
    HAL_GPIO_Init(port, &GPIO_InitStruct);
}

void stm_pin_write(struct pin_device *dev, pin_index_t pin, pin_val_t val)
{
    if(val == PIN_HIGH)
        HAL_GPIO_WritePin(stm_pin_get_port(STM_PORT(pin)), pin_defines[STM_PIN(pin)], GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(stm_pin_get_port(STM_PORT(pin)), pin_defines[STM_PIN(pin)], GPIO_PIN_RESET);
}


int stm_pin_read(struct pin_device *dev, pin_index_t pin)
{
	if(HAL_GPIO_ReadPin(stm_pin_get_port(STM_PORT(pin)), pin_defines[STM_PIN(pin)])==GPIO_PIN_SET)
		return PIN_HIGH;
	else
		return PIN_LOW;
}

static const struct pin_operation ops={
    .pin_mode = stm_pin_mode,
    .pin_write = stm_pin_write,
    .pin_read = stm_pin_read,
};

int stm_pin_init(void)
{
    int ret;
    
    ret = pin_device_register("pin", &ops);
    
    if(ret<0)
    {
        LOG_ERROR("pin register failed!");
        return -1;
    }
    
    return 0;
}

