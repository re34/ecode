#include "bsp_pin.h"
#include "stdio.h"


const uint32_t pin_defines[16] = {
    LL_GPIO_PIN_0,
    LL_GPIO_PIN_1,
    LL_GPIO_PIN_2,
    LL_GPIO_PIN_3,
    LL_GPIO_PIN_4,
    LL_GPIO_PIN_5,
    LL_GPIO_PIN_6,
    LL_GPIO_PIN_7,
    LL_GPIO_PIN_8,
    LL_GPIO_PIN_9,
    LL_GPIO_PIN_10,
    LL_GPIO_PIN_11,
    LL_GPIO_PIN_12,
    LL_GPIO_PIN_13,
    LL_GPIO_PIN_14,
    LL_GPIO_PIN_15
};

static inline GPIO_TypeDef *bsp_pin_get_port(uint32_t port_idx)
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


GPIO_TypeDef *bsp_gpio_set_clk(uint32_t port_idx) {
    uint32_t gpio_add = 0;
    switch (port_idx) {
        case PortA:
            gpio_add = GPIOA_BASE;
            __GPIOA_CLK_ENABLE();
            break;
        case PortB:
            gpio_add = GPIOB_BASE;
            __GPIOB_CLK_ENABLE();
            break;
#if defined(GPIOC_BASE)
        case PortC:
            gpio_add = GPIOC_BASE;
            __GPIOC_CLK_ENABLE();
            break;
#endif
#if defined GPIOD_BASE
        case PortD:
            gpio_add = GPIOD_BASE;
            __GPIOD_CLK_ENABLE();
            break;
#endif
#if defined GPIOE_BASE
        case PortE:
            gpio_add = GPIOE_BASE;
            __GPIOE_CLK_ENABLE();
            break;
#endif
#if defined GPIOF_BASE
        case PortF:
            gpio_add = GPIOF_BASE;
            __GPIOF_CLK_ENABLE();
            break;
#endif
#if defined GPIOG_BASE
        case PortG:
//#if defined TARGET_STM32L4
            __HAL_RCC_PWR_CLK_ENABLE();
            HAL_PWREx_EnableVddIO2();
//#endif
            gpio_add = GPIOG_BASE;
            __GPIOG_CLK_ENABLE();
            break;
#endif
#if defined GPIOH_BASE
        case PortH:
            gpio_add = GPIOH_BASE;
            __GPIOH_CLK_ENABLE();
            break;
#endif
#if defined GPIOI_BASE
        case PortI:
            gpio_add = GPIOI_BASE;
            __GPIOI_CLK_ENABLE();
            break;
#endif
#if defined GPIOJ_BASE
        case PortJ:
            gpio_add = GPIOJ_BASE;
            __GPIOJ_CLK_ENABLE();
            break;
#endif
#if defined GPIOK_BASE
        case PortK:
            gpio_add = GPIOK_BASE;
            __GPIOK_CLK_ENABLE();
            break;
#endif
        default:
            printf("Pinmap error: wrong port number.");
            break;
    }
    return (GPIO_TypeDef *) gpio_add;
}


void bsp_pin_digital_out_init(pin_name_t pin, uint8_t pull)
{
    GPIO_TypeDef *port;
    uint32_t pin_mask = pin_defines[STM_PIN(pin)];
    
    port = bsp_gpio_set_clk(STM_PORT(pin));
    
    LL_GPIO_SetPinMode(port, pin_mask, LL_GPIO_MODE_OUTPUT);
    
    LL_GPIO_SetPinOutputType(port, pin_mask, LL_GPIO_OUTPUT_PUSHPULL);
    
    LL_GPIO_SetPinSpeed(port, pin_mask, LL_GPIO_SPEED_HIGH);
    
    if(pull==PIN_PULLUP)
        LL_GPIO_SetPinPull(port, pin_mask, LL_GPIO_PULL_UP);
    else
        LL_GPIO_SetPinPull(port, pin_mask, LL_GPIO_PULL_DOWN);  
}


void bsp_pin_set(pin_name_t pin)
{
    LL_GPIO_SetOutputPin(bsp_pin_get_port(STM_PORT(pin)), pin_defines[STM_PIN(pin)]);
}

void bsp_pin_reset(pin_name_t pin)
{
    LL_GPIO_ResetOutputPin(bsp_pin_get_port(STM_PORT(pin)), pin_defines[STM_PIN(pin)]);    
}

void bsp_pin_toggle(pin_name_t pin)
{
    LL_GPIO_TogglePin(bsp_pin_get_port(STM_PORT(pin)), pin_defines[STM_PIN(pin)]);
}
