#include "usart.h"


struct stm32_uart{
    USART_TypeDef *instance;
    IRQn_Type irq;
    struct stm32_uart_dma
    {
        DMA_Stream_TypeDef *rx_stream;
        uint32_t rx_ch;
        uint32_t rx_flag;
        uint8_t rx_irq_ch;
        e_size_t setting_recv_len;
        e_size_t last_recv_index;
    }dma;
};



static int stm32_putc()
{
    
}