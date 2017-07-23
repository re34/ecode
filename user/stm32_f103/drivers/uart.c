#include "uart.h"
#include "board_includes.h"
#include "ecode.h"

#define USE_UART1
#define UART1_INSTANCE           USART1
#define UART1_CLK_ENABLE()     __HAL_RCC_USART1_CLK_ENABLE()
#define UART1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOA_CLK_ENABLE()
#define UART1_TX_PIN               GPIO_PIN_9
#define UART1_TX_GPIO_PORT              GPIOA
#define UART1_RX_PIN               GPIO_PIN_10
#define UART1_RX_GPIO_PORT              GPIOA
#define UART1_IRQ_NUM              USART1_IRQn
#define UART1_INTERRUPT              USART1_IRQHandler
#define SERIAL_RX_BUFFER_SIZE       100



#define UART_ENABLE_IRQ(n)            NVIC_EnableIRQ((n))
#define UART_DISABLE_IRQ(n)           NVIC_DisableIRQ((n))

struct stm_uart{
    USART_TypeDef *instance;
    UART_HandleTypeDef uart_handle;
    IRQn_Type irq;
};


static e_err_t stm_uart_init(struct serial_dev *serial)
{
    struct stm_uart *uart;
    struct serial_configure *cfg;
    UART_HandleTypeDef *huart;
    
    ASSERT_PARAM(serial!=NULL);
    cfg = &serial->config;
    
    uart = (struct stm_uart *)serial->private_data;
    huart = &uart->uart_handle;
    huart->Instance = uart->instance;
    
    if(cfg->data_bits==DATA_BITS_8)
    {
        huart->Init.WordLength = UART_WORDLENGTH_8B;
    }else if(cfg->data_bits==DATA_BITS_9){
        huart->Init.WordLength = UART_WORDLENGTH_9B;
    }
    
    if(cfg->stop_bits == STOP_BITS_1){
        huart->Init.StopBits = UART_STOPBITS_1;
    }else if(cfg->stop_bits==STOP_BITS_2){
        huart->Init.StopBits = UART_STOPBITS_2;
    }
    
    if(cfg->parity==PARITY_NONE)
    {
        huart->Init.Parity = UART_PARITY_NONE;
    }else if(cfg->parity==PARITY_ODD){
        huart->Init.Parity = UART_PARITY_ODD;
    }else if(cfg->parity==PARITY_EVEN){
        huart->Init.Parity = UART_PARITY_EVEN;
    }
        
    /* TX/RX direction */
    huart->Init.Mode = UART_MODE_TX_RX;
    
    huart->Init.OverSampling = UART_OVERSAMPLING_16;

    /* 8 data bit, 1 start bit, 1 stop bit, no parity */
    //LL_USART_ConfigCharacter(UART1_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

    /* No Hardware Flow control */
    /* Reset value is LL_USART_HWCONTROL_NONE */
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;

    huart->Init.BaudRate = cfg->baud_rate;

    if(HAL_UART_Init(huart)!=HAL_OK)
    {
        LOG_ERROR("uart hal driver init failed!");
        return E_ERROR;
    }
    
    __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_RXNE);
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TC);

    return E_EOK;
}

static void uart_isr(struct serial_dev *serial)
{
    struct stm_uart * uart= (struct stm_uart *)serial->private_data;
    
    ASSERT_PARAM(uart!=NULL);
    
    if(__HAL_UART_GET_FLAG(&uart->uart_handle,UART_FLAG_RXNE))
    {
        serial_hw_isr(serial, SERIAL_EVENT_RX_IND);
        __HAL_UART_CLEAR_FLAG(&uart->uart_handle, UART_FLAG_RXNE);
    }

}


static int stm_putc(struct serial_dev *serial, char c)
{
    struct stm_uart *uart;

    ASSERT_PARAM(serial != NULL);
    uart = serial->private_data;
    ASSERT_PARAM(uart!=NULL);
    
    
    HAL_UART_Transmit(&uart->uart_handle, (uint8_t *)&c, 1, 0xFFFF);
    
    return 1;
}

static int stm_getc(struct serial_dev *serial)
{
    int ch;
    struct stm_uart *uart;

    ASSERT_PARAM(serial != NULL);
    uart = (struct stm_uart *)serial->private_data;

    ch = -1;
    
    if(__HAL_UART_GET_FLAG(&uart->uart_handle,UART_FLAG_RXNE))
        ch = uart->instance->DR&0xFF;
    //HAL_UART_Receive(&uart->uart_handle, (uint8_t *)&ch, 1, 0);

    return ch;
}

static const struct serial_operation stm_uart_ops={
    .init = stm_uart_init,
    .fputc = stm_putc,
    .fgetc = stm_getc,
};

#ifdef USE_UART1
struct stm_uart uart1 = {
    .instance = UART1_INSTANCE,
    .irq = UART1_IRQ_NUM,
};

e_uint8_t serial1_rx_buffer[SERIAL_RB_BUFSZ];

struct serial_rx_fifo serial1_rx_fifo={
    .buffer = serial1_rx_buffer,
    .put_index = 0,
    .get_index = 0,
    .bufsz = SERIAL_RB_BUFSZ,
};

static struct serial_dev serial1={
    .flag = SERIAL_FLAG_INT_RX,
    
    .ops = &stm_uart_ops,
    .config = SERIAL_CONFIG_DEFAULT,
    
    .serial_rx = &serial1_rx_fifo,
    
    .private_data = &uart1,
};


void UART1_INTERRUPT(void)
{
    uart_isr(&serial1);
}
#endif

static void uart_rcc_configuration(void)
{
#ifdef USE_UART1
    UART1_CLK_ENABLE();
    UART1_GPIO_CLK_ENABLE();
#endif
}

static void uart_gpio_configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
#ifdef USE_UART1
  
    /*##-2- Configure peripheral GPIO ##########################################*/
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = UART1_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(UART1_TX_GPIO_PORT, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = UART1_RX_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    HAL_GPIO_Init(UART1_RX_GPIO_PORT, &GPIO_InitStruct);
#endif
}

static void uart_nvic_configuration(struct stm_uart *uart)
{
    NVIC_SetPriority(uart->irq, 0);
    NVIC_EnableIRQ(uart->irq);
}

void uart_hw_init(void)
{
    struct stm_uart *uart;
    struct serial_configure config = SERIAL_CONFIG_DEFAULT;
    
    uart_rcc_configuration();
    uart_gpio_configuration();
    
#ifdef USE_UART1
    uart = &uart1;
    
    serial1.ops = &stm_uart_ops;
    serial1.config = config;
    
    uart_nvic_configuration(uart);
    
    serial_register(COM1,
                    &serial1,
                    "COM1");
#endif
}

