#include "uart.h"
#include "board_includes.h"
#include "ecode.h"

#define USE_UART3
#define UART3_INSTANCE           USART3
#define UART3_CLK_ENABLE()     __HAL_RCC_USART3_CLK_ENABLE()
#define UART3_CLK_SOURCE()     //LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1)
#define UART3_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOD_CLK_ENABLE()
#define UART3_TX_PIN               GPIO_PIN_8
#define UART3_TX_GPIO_PORT              GPIOD
//#define UART3_SET_TX_GPIO_AF()         LL_GPIO_SetAFPin_0_7(UART3_TX_GPIO_PORT, UART3_TX_PIN, LL_GPIO_AF_7)
#define UART3_RX_PIN               GPIO_PIN_9
#define UART3_RX_GPIO_PORT              GPIOD
//#define UART3_SET_RX_GPIO_AF()         LL_GPIO_SetAFPin_0_7(UART3_RX_GPIO_PORT, UART3_RX_PIN, LL_GPIO_AF_7)
#define UART3_AF_CFG                    GPIO_AF7_USART3
#define UART3_IRQ_NUM              USART3_IRQn
#define UART3_INTERRUPT              USART3_IRQHandler
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
    
    while(!LL_USART_IsActiveFlag_TXE(uart->instance));
    LL_USART_TransmitData8(uart->instance, c);
    
    return 1;
}

static int stm_getc(struct serial_dev *serial)
{
    int ch;
    struct stm_uart *uart;

    ASSERT_PARAM(serial != NULL);
    uart = (struct stm_uart *)serial->private_data;

    ch = -1;
    
    if(LL_USART_IsActiveFlag_RXNE(uart->instance))
    {
        ch = LL_USART_ReceiveData8(uart->instance);
    }

    return ch;
}

static const struct serial_operation stm_uart_ops={
    .init = stm_uart_init,
    .fputc = stm_putc,
    .fgetc = stm_getc,
};

#ifdef USE_UART3
struct stm_uart uart3 = {
    .instance = UART3_INSTANCE,
    .irq = UART3_IRQ_NUM,
};

e_uint8_t serial3_rx_buffer[SERIAL_RB_BUFSZ];

struct serial_rx_fifo serial3_rx_fifo={
    .buffer = serial3_rx_buffer,
    .put_index = 0,
    .get_index = 0,
    .bufsz = SERIAL_RB_BUFSZ,
};

static struct serial_dev serial3={
    .flag = SERIAL_FLAG_INT_RX,
    
    .ops = &stm_uart_ops,
    .config = SERIAL_CONFIG_DEFAULT,
    
    .serial_rx = &serial3_rx_fifo,
    
    .private_data = &uart3,
};


void UART3_INTERRUPT(void)
{
    uart_isr(&serial3);
}
#endif

static void uart_rcc_configuration(void)
{
#ifdef USE_UART3
    UART3_CLK_ENABLE();
    UART3_CLK_SOURCE();
    UART3_GPIO_CLK_ENABLE();
#endif
}

static void uart_gpio_configuration(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;
#ifdef USE_UART3
  
    /*##-2- Configure peripheral GPIO ##########################################*/
    /* UART TX GPIO pin configuration  */
    GPIO_InitStruct.Pin       = UART3_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = UART3_AF_CFG;

    HAL_GPIO_Init(UART3_TX_GPIO_PORT, &GPIO_InitStruct);

    /* UART RX GPIO pin configuration  */
    GPIO_InitStruct.Pin = UART3_RX_PIN;
    //GPIO_InitStruct.Mode    = GPIO_MODE_INPUT;
    //GPIO_InitStruct.Pull    = GPIO_NOPULL;
    HAL_GPIO_Init(UART3_RX_GPIO_PORT, &GPIO_InitStruct);
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
    
#ifdef USE_UART3
    uart = &uart3;
    
    serial3.ops = &stm_uart_ops;
    serial3.config = config;
    
    uart_nvic_configuration(uart);
    
    serial_register(COM1,
                    &serial3,
                    "COM1");
#endif
}

