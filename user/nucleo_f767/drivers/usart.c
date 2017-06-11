#include "usart.h"
#include "board_includes.h"
#include "ecode.h"

#define UART3_INSTANCE           USART3
#define UART3_CLK_ENABLE()     LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART3)
#define UART3_CLK_SOURCE()     LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1)
#define UART3_GPIO_CLK_ENABLE()    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD)
#define UART3_TX_PIN               LL_GPIO_PIN_8
#define UART3_TX_GPIO_PORT              GPIOD
#define UART3_SET_TX_GPIO_AF()         LL_GPIO_SetAFPin_8_15(UART3_TX_GPIO_PORT, UART3_TX_PIN, LL_GPIO_AF_7)
#define UART3_RX_PIN               LL_GPIO_PIN_9
#define UART3_RX_GPIO_PORT              GPIOD
#define UART3_SET_RX_GPIO_AF()         LL_GPIO_SetAFPin_8_15(UART3_RX_GPIO_PORT,UART3_RX_PIN, LL_GPIO_AF_7)
#define UART3_IRQ_NUM               USART3_IRQn
#define UART3_INTERRUPT             USART3_IRQHandler


#define UART3_ENABLE_IRQ(n)            NVIC_EnableIRQ((n))
#define UART3_DISABLE_IRQ(n)           NVIC_DisableIRQ((n))

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



static e_err_t stm32_configure(struct serial_dev *serial, struct serial_configure *cfg)
{
    struct stm32_uart *uart;
    
    ASSERT_PARAM(serial!=NULL);
    ASSERT_PARAM(cfg!=NULL);
    
    uart = (struct stm32_uart *)serial->parent.private_data;
    
    if(cfg->data_bits==DATA_BITS_8)
    {
        LL_USART_SetDataWidth(uart->instance,LL_USART_DATAWIDTH_8B);
    }else if(cfg->data_bits==DATA_BITS_9){
        LL_USART_SetDataWidth(uart->instance,LL_USART_DATAWIDTH_9B);
    }
    
    if(cfg->stop_bits == STOP_BITS_1){
        LL_USART_SetStopBitsLength(uart->instance, LL_USART_STOPBITS_1);
    }else if(cfg->stop_bits==STOP_BITS_2){
        LL_USART_SetStopBitsLength(uart->instance, LL_USART_STOPBITS_2);
    }
    
    if(cfg->parity==PARITY_NONE)
    {
        LL_USART_SetParity(uart->instance, LL_USART_PARITY_NONE);
    }else if(cfg->parity==PARITY_ODD){
        LL_USART_SetParity(uart->instance, LL_USART_PARITY_ODD);
    }else if(cfg->parity==PARITY_EVEN){
        LL_USART_SetParity(uart->instance, LL_USART_PARITY_EVEN);
    }
        
    /* TX/RX direction */
    LL_USART_SetTransferDirection(UART3_INSTANCE, LL_USART_DIRECTION_TX_RX);

    /* 8 data bit, 1 start bit, 1 stop bit, no parity */
    //LL_USART_ConfigCharacter(UART3_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

    /* No Hardware Flow control */
    /* Reset value is LL_USART_HWCONTROL_NONE */
    LL_USART_SetHWFlowCtrl(UART3_INSTANCE, LL_USART_HWCONTROL_NONE);
    if(uart->instance==USART3)
        LL_USART_SetBaudRate(UART3_INSTANCE, SystemCoreClock/4, LL_USART_OVERSAMPLING_16, cfg->baud_rate); 

    LL_USART_Enable(UART3_INSTANCE);
    
    return E_EOK;
}


static e_err_t stm32_control(struct serial_dev *serial, int cmd, void *arg)
{
    struct stm32_uart *uart;
    e_uint32_t ctrl_arg = (e_uint32_t)(arg);
    
    ASSERT_PARAM(serial!=NULL);
    
    uart = (struct stm32_uart *)serial->parent.private_data;
    switch(cmd)
    {
    case DEVICE_CTRL_CLR_INT:
        UART3_DISABLE_IRQ(uart->irq);
        LL_USART_DisableIT_RXNE(uart->instance);
        break;
    case DEVICE_CTRL_SET_INT:
        UART3_ENABLE_IRQ(uart->irq);
        LL_USART_EnableIT_RXNE(uart->instance);
        break;
    }
    return E_EOK;
}

static int stm32_putc(struct serial_dev *serial, char c)
{
    struct stm32_uart *uart;
    
    ASSERT_PARAM(serial!=NULL);
    
    uart = (struct stm32_uart *)serial->parent.private_data;
    
    while(LL_USART_IsActiveFlag_TXE(uart->instance)==RESET);
    LL_USART_TransmitData8(uart->instance, c);
    return c;
}

static int stm32_getc(struct serial_dev *serial)
{
    int ch;
    struct stm32_uart *uart;
    
    ASSERT_PARAM(serial!=NULL);
    
    uart = (struct stm32_uart *)serial->parent.private_data;
    
    ch = -1;
    if(LL_USART_IsActiveFlag_RXNE(uart->instance)!=RESET)
    {
        ch = LL_USART_ReceiveData8(uart->instance);
    }
    
    return ch;
}


static void uart_isr(struct serial_dev *serial)
{
    struct stm32_uart * uart= (struct stm32_uart *)serial->parent.private_data;
    
    ASSERT_PARAM(uart!=NULL);
    
    if(LL_USART_IsActiveFlag_RXNE(uart->instance))
    {
        serial_hw_isr(serial, SERIAL_EVENT_RX_IND);
    }
    if(LL_USART_IsActiveFlag_TC(uart->instance))
    {
        
    }
}

static const struct serial_operation stm_uart_ops={
    .configure = stm32_configure,
    .control = stm32_control,
    .putc = stm32_putc,
    .getc = stm32_getc,
};




struct stm32_uart uart3 = {
    .instance = USART3,
    .irq = USART3_IRQn,
};

e_uint8_t serial3_rx_buffer[SERIAL_RB_BUFSZ];

struct serial_rx_fifo serial3_rx_fifo={
    .buffer = serial3_rx_buffer,
    .put_index = 0,
    .get_index = 0,
};

static struct serial_dev serial3={
    .serial_rx = &serial3_rx_fifo,
};


void USART3_IRQHandler(void)
{
    uart_isr(&serial3);
}

static void usart_rcc_configuration(void)
{
    UART3_CLK_ENABLE();
    UART3_CLK_SOURCE();
    UART3_GPIO_CLK_ENABLE();
}

static void usart_gpio_configuration(void)
{
  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(UART3_TX_GPIO_PORT, UART3_TX_PIN, LL_GPIO_MODE_ALTERNATE);
  UART3_SET_TX_GPIO_AF();
  LL_GPIO_SetPinSpeed(UART3_TX_GPIO_PORT, UART3_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(UART3_TX_GPIO_PORT, UART3_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(UART3_TX_GPIO_PORT, UART3_TX_PIN, LL_GPIO_PULL_UP);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(UART3_RX_GPIO_PORT, UART3_RX_PIN, LL_GPIO_MODE_ALTERNATE);
  UART3_SET_RX_GPIO_AF();
  LL_GPIO_SetPinSpeed(UART3_RX_GPIO_PORT, UART3_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(UART3_RX_GPIO_PORT, UART3_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(UART3_RX_GPIO_PORT, UART3_RX_PIN, LL_GPIO_PULL_UP);
}

static void usart_nvic_configuration(struct stm32_uart *uart)
{
    NVIC_SetPriority(uart->irq, 0);
    NVIC_EnableIRQ(uart->irq);
    
    //LL_USART_EnableIT_RXNE(uart->instance);
    //LL_USART_ClearFlag_TC(uart->instance);
}

void usart_hw_init(void)
{
    struct stm32_uart *uart;
    struct serial_configure config = SERIAL_CONFIG_DEFAULT;
    
    usart_rcc_configuration();
    usart_gpio_configuration();
    
    uart = &uart3;
    
    serial3.ops = &stm_uart_ops;
    serial3.config = config;
    
    usart_nvic_configuration(uart);
    
    serial_register(COM1,
                    &serial3,
                    "COM1",
                    DEVICE_FLAG_INT_RX,
                    uart);
                    
    device_open(&(serial3.parent), DEVICE_FLAG_INT_RX);
}