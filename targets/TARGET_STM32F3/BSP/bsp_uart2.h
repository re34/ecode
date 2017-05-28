#ifndef __BSP_UART2_H__
#define __BSP_UART2_H__
#include "target_chip.h"

#define UART2_INSTANCE           USART1
#define UART2_CLK_ENABLE()     LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1)
#define UART2_CLK_SOURCE()     LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK2)
#define UART2_GPIO_CLK_ENABLE()    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA)
#define UART2_TX_PIN               LL_GPIO_PIN_9
#define UART2_TX_GPIO_PORT              GPIOA
#define UART2_SET_TX_GPIO_AF()         LL_GPIO_SetAFPin_8_15(UART2_TX_GPIO_PORT, UART2_TX_PIN, LL_GPIO_AF_7)
#define UART2_RX_PIN               LL_GPIO_PIN_10
#define UART2_RX_GPIO_PORT              GPIOA
#define UART2_SET_RX_GPIO_AF()         LL_GPIO_SetAFPin_8_15(UART2_RX_GPIO_PORT,UART2_RX_PIN, LL_GPIO_AF_7)
#define UART2_IRQ_NUM              USART1_IRQn
#define UART2_INTERRUPT              USART1_IRQHandler



typedef struct{
    void (*uart_rxne_callback)(void);
    void (*uart_tc_callback)(void);
}uart2_irq_t;

int bsp_uart2_init(uint32_t baudrate);
int bsp_uart2_getbyte(void);
int bsp_uart2_writebyte(uint8_t byte);
void bsp_uart2_register_irq(uart2_irq_t irq);

#endif