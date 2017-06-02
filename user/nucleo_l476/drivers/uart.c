#include "uart.h"
#include "board_includes.h"
#include "ecode.h"

#define UART_INSTANCE           USART2
#define UART_CLK_ENABLE()     LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2)
#define UART_CLK_SOURCE()     LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1)
#define UART_GPIO_CLK_ENABLE()    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA)
#define UART_TX_PIN               LL_GPIO_PIN_2
#define UART_TX_GPIO_PORT              GPIOA
#define UART_SET_TX_GPIO_AF()         LL_GPIO_SetAFPin_0_7(UART_TX_GPIO_PORT, UART_TX_PIN, LL_GPIO_AF_7)
#define UART_RX_PIN               LL_GPIO_PIN_3
#define UART_RX_GPIO_PORT              GPIOA
#define UART_SET_RX_GPIO_AF()         LL_GPIO_SetAFPin_0_7(UART_RX_GPIO_PORT, UART_RX_PIN, LL_GPIO_AF_7)
#define UART_IRQ_NUM              USART2_IRQn
#define UART_INTERRUPT              USART2_IRQHandler
#define SERIAL_RX_BUFFER_SIZE       100


static struct serial_dev serial;


static Fifo serial_rx_fifo;
static unsigned char serial_rx_buffer[SERIAL_RX_BUFFER_SIZE];

static int uart_getc(void);
static int uart_in_waiting(void);


static void uart_hw_init(int baudrate)
{
  /* (1) Enable GPIO clock and configures the USART pins *********************/

  /* Enable the peripheral clock of GPIO Port */
  UART_GPIO_CLK_ENABLE();

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(UART_TX_GPIO_PORT, UART_TX_PIN, LL_GPIO_MODE_ALTERNATE);
  UART_SET_TX_GPIO_AF();
  LL_GPIO_SetPinSpeed(UART_TX_GPIO_PORT, UART_TX_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(UART_TX_GPIO_PORT, UART_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(UART_TX_GPIO_PORT, UART_TX_PIN, LL_GPIO_PULL_UP);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(UART_RX_GPIO_PORT, UART_RX_PIN, LL_GPIO_MODE_ALTERNATE);
  UART_SET_RX_GPIO_AF();
  LL_GPIO_SetPinSpeed(UART_RX_GPIO_PORT, UART_RX_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(UART_RX_GPIO_PORT, UART_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(UART_RX_GPIO_PORT, UART_RX_PIN, LL_GPIO_PULL_UP);

  NVIC_SetPriority(UART_IRQ_NUM, 0);
  NVIC_EnableIRQ(UART_IRQ_NUM);
  /* (2) Enable USART peripheral clock and clock source ***********************/
  UART_CLK_ENABLE();

  /* Set clock source */
  UART_CLK_SOURCE();

  /* (3) Configure USART functional parameters ********************************/
  
  /* Disable USART prior modifying configuration registers */
  /* Note: Commented as corresponding to Reset value */
  // LL_USART_Disable(UART_INSTANCE);

  /* TX/RX direction */
  LL_USART_SetTransferDirection(UART_INSTANCE, LL_USART_DIRECTION_TX_RX);

  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(UART_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

  /* No Hardware Flow control */
  /* Reset value is LL_USART_HWCONTROL_NONE */
   LL_USART_SetHWFlowCtrl(UART_INSTANCE, LL_USART_HWCONTROL_NONE);

  /* Oversampling by 16 */
  /* Reset value is LL_USART_OVERSAMPLING_16 */
  // LL_USART_SetOverSampling(UART_INSTANCE, LL_USART_OVERSAMPLING_16);

  /* Set Baudrate to 115200 using APB frequency set to 80000000 Hz */
  /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
  /* Ex :
      Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance
  
      In this example, Peripheral Clock is expected to be equal to 80000000 Hz => equal to SystemCoreClock
  */
  LL_USART_SetBaudRate(UART_INSTANCE, SystemCoreClock, LL_USART_OVERSAMPLING_16, baudrate); 

  /* (4) Enable USART *********************************************************/
  LL_USART_Enable(UART_INSTANCE);
  
  /* Polling USART initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(UART_INSTANCE))) || (!(LL_USART_IsActiveFlag_REACK(UART_INSTANCE))))
  { 
  } 

  LL_USART_EnableIT_RXNE(UART_INSTANCE);
  LL_USART_ClearFlag_TC(UART_INSTANCE);

}

int uart_init(void)
{
    serial.baudrate = 115200;
    
    if(FifoCreate(&serial_rx_fifo, serial_rx_buffer, SERIAL_RX_BUFFER_SIZE)<0)
        return -1;
    
    uart_hw_init(serial.baudrate);

    serial.put_c = uart_putc;
    serial.get_c = uart_getc;
    serial.in_waiting = uart_in_waiting;
    if(serial_register(COM1, &serial)<0)
        return -2;

    return 0;
}

int uart_putc(unsigned char c)
{
    LL_USART_TransmitData8(UART_INSTANCE, c);
    
    while(LL_USART_IsActiveFlag_TC(UART_INSTANCE)==RESET);
    
    return c;
}

static int uart_getc(void)
{
    while(FifoIsEmpty(&serial_rx_fifo));
    return FifoPop(&serial_rx_fifo);
}

static int uart_in_waiting(void)
{
    return FifoQueryPushed(&serial_rx_fifo);
}

void UART_INTERRUPT(void)
{
    int data;
    
    if(LL_USART_IsActiveFlag_RXNE(UART_INSTANCE))
    {
        data = LL_USART_ReceiveData8(UART_INSTANCE);
        if(FifoIsFull(&serial_rx_fifo)!=1)
            FifoPush(&serial_rx_fifo, data); 
    }
}

