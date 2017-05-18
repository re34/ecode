#ifndef __bsp_uart11_H__
#define __bsp_uart11_H__
#include "target_chip.h"


#define USART1_CLK_ENABLE()     LL_APB1_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1)
#define USART1_CLK_SOURCE()     LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2)
#define USART1_GPIO_CLK_ENABLE()    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#define USART1_TX_PIN               LL_GPIO_PIN_9
#define USART1_TX_GPIO_PORT              GPIOA
#define USART1_SET_TX_GPIO_AF()         LL_GPIO_SetAFPin_0_7(USART1_TX_GPIO_PORT, LL_GPIO_PIN_9, LL_GPIO_AF_7)
#define USART1_RX_PIN               LL_GPIO_PIN_10
#define USART1_RX_GPIO_PORT              GPIOA
#define USART1_SET_RX_GPIO_AF()         LL_GPIO_SetAFPin_0_7(USART1_RX_GPIO_PORT,LL_GPIO_PIN_10, LL_GPIO_AF_7)
#define USART1_IRQ_NUM              USART1_IRQn



typedef struct{
    void (*uart1_rxne_callback)(void);
    void (*uart1_tc_callback)(void);
}uart1_irq_t;

int bsp_uart1_init(uint32_t baudrate);
int bsp_uart1_getbyte(void);
int bsp_uart1_writebyte(uint8_t byte);
void bsp_uart1_register_irq(uart1_irq_t irq);

#endif