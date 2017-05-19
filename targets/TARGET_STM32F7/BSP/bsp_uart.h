#ifndef __BSP_UART_H__
#define __BSP_UART_H__
#include "target_chip.h"


#define USART3_CLK_ENABLE()     LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)
#define USART3_CLK_SOURCE()     LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1)
#define USART3_GPIO_CLK_ENABLE()    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD)
#define USART3_TX_PIN               LL_GPIO_PIN_8
#define USART3_TX_GPIO_PORT              GPIOD
#define USART3_SET_TX_GPIO_AF()         LL_GPIO_SetAFPin_8_15(USART3_TX_GPIO_PORT, USART3_TX_PIN, LL_GPIO_AF_7)
#define USART3_RX_PIN               LL_GPIO_PIN_9
#define USART3_RX_GPIO_PORT              GPIOD
#define USART3_SET_RX_GPIO_AF()         LL_GPIO_SetAFPin_8_15(USART3_RX_GPIO_PORT,USART3_RX_PIN, LL_GPIO_AF_7)
#define USART3_IRQ_NUM              USART3_IRQn



typedef struct{
    void (*uart_rxne_callback)(void);
    void (*uart_tc_callback)(void);
}uart_irq_t;

int bsp_uart_init(uint32_t baudrate);
int bsp_uart_getbyte(void);
int bsp_uart_writebyte(uint8_t byte);
void bsp_uart_register_irq(uart_irq_t irq);

#endif