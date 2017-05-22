#ifndef __BSP_UART_H__
#define __BSP_UART_H__
#include "target_chip.h"

#define UART_INSTANCE           USART3
#define UART_CLK_ENABLE()     LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)
#define UART_CLK_SOURCE()     LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1)
#define UART_GPIO_CLK_ENABLE()    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD)
#define UART_TX_PIN               LL_GPIO_PIN_8
#define UART_TX_GPIO_PORT              GPIOD
#define UART_SET_TX_GPIO_AF()         LL_GPIO_SetAFPin_8_15(UART_TX_GPIO_PORT, UART_TX_PIN, LL_GPIO_AF_7)
#define UART_RX_PIN               LL_GPIO_PIN_9
#define UART_RX_GPIO_PORT              GPIOD
#define UART_SET_RX_GPIO_AF()         LL_GPIO_SetAFPin_8_15(UART_RX_GPIO_PORT,UART_RX_PIN, LL_GPIO_AF_7)
#define UART_IRQ_NUM               USART3_IRQn
#define UART_INTERRUPT             USART3_IRQHandler


typedef struct{
    void (*uart_rxne_callback)(void);
    void (*uart_tc_callback)(void);
}uart_irq_t;

int bsp_uart_init(uint32_t baudrate);
int bsp_uart_getbyte(void);
int bsp_uart_writebyte(uint8_t byte);
void bsp_uart_register_irq(uart_irq_t irq);

#endif