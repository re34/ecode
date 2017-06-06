#include "uart2.h"
#include "board_includes.h"
#include "ecode.h"

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


#define SERIAL_RX_BUFFER_SIZE       100


static struct serial_dev serial;


static Fifo serial_rx_fifo;
static unsigned char serial_rx_buffer[SERIAL_RX_BUFFER_SIZE];

static int uart2_getc(void);
static int uart2_in_waiting(void);


static void uart2_hw_init(int baudrate)
{
  /* (1) Enable GPIO clock and configures the USART pins *********************/

  /* Enable the peripheral clock of GPIO Port */
  UART2_GPIO_CLK_ENABLE();

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(UART2_TX_GPIO_PORT, UART2_TX_PIN, LL_GPIO_MODE_ALTERNATE);
  UART2_SET_TX_GPIO_AF();
  LL_GPIO_SetPinSpeed(UART2_TX_GPIO_PORT, UART2_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(UART2_TX_GPIO_PORT, UART2_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(UART2_TX_GPIO_PORT, UART2_TX_PIN, LL_GPIO_PULL_UP);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(UART2_RX_GPIO_PORT, UART2_RX_PIN, LL_GPIO_MODE_ALTERNATE);
  UART2_SET_RX_GPIO_AF();
  LL_GPIO_SetPinSpeed(UART2_RX_GPIO_PORT, UART2_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(UART2_RX_GPIO_PORT, UART2_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(UART2_RX_GPIO_PORT, UART2_RX_PIN, LL_GPIO_PULL_UP);

  NVIC_SetPriority(UART2_IRQ_NUM, 0);
  NVIC_EnableIRQ(UART2_IRQ_NUM);
  /* (2) Enable USART peripheral clock and clock source ***********************/
  UART2_CLK_ENABLE();

  /* Set clock source */
  UART2_CLK_SOURCE();

  /* (3) Configure USART functional parameters ********************************/
  
  /* Disable USART prior modifying configuration registers */
  /* Note: Commented as corresponding to Reset value */
  // LL_USART_Disable(UART2_INSTANCE);

  /* TX/RX direction */
  LL_USART_SetTransferDirection(UART2_INSTANCE, LL_USART_DIRECTION_TX_RX);

  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(UART2_INSTANCE, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

  /* No Hardware Flow control */
  /* Reset value is LL_USART_HWCONTROL_NONE */
   LL_USART_SetHWFlowCtrl(UART2_INSTANCE, LL_USART_HWCONTROL_NONE);

  /* Oversampling by 16 */
  /* Reset value is LL_USART_OVERSAMPLING_16 */
  // LL_USART_SetOverSampling(UART2_INSTANCE, LL_USART_OVERSAMPLING_16);

  /* Set Baudrate to 115200 using APB frequency set to 80000000 Hz */
  /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
  /* Ex :
      Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUART2ClockFreq(Instance); depending on USART/UART2 instance
  
      In this example, Peripheral Clock is expected to be equal to 80000000 Hz => equal to SystemCoreClock
  */
  LL_USART_SetBaudRate(UART2_INSTANCE, SystemCoreClock, LL_USART_OVERSAMPLING_16, baudrate); 

  /* (4) Enable USART *********************************************************/
  LL_USART_Enable(UART2_INSTANCE);

  /* Polling USART initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(UART2_INSTANCE))) || (!(LL_USART_IsActiveFlag_REACK(UART2_INSTANCE))))
  { 
  } 

  LL_USART_EnableIT_RXNE(UART2_INSTANCE);
  LL_USART_ClearFlag_TC(UART2_INSTANCE);
}

int uart2_init(void)
{
    serial.baudrate = 115200;
    
    if(FifoCreate(&serial_rx_fifo, serial_rx_buffer, SERIAL_RX_BUFFER_SIZE)<0)
        return -1;
    
    uart2_hw_init(serial.baudrate);

    serial.put_c = uart2_putc;
    serial.get_c = uart2_getc;
    serial.in_waiting = uart2_in_waiting;
    if(serial_register(COM2, &serial)<0)
        return -2;

    return 0;
}

int uart2_putc(unsigned char c)
{
    LL_USART_TransmitData8(UART2_INSTANCE, c);
    
    while(LL_USART_IsActiveFlag_TC(UART2_INSTANCE)==RESET);
    
    return c;
}

static int uart2_getc(void)
{
    while(FifoIsEmpty(&serial_rx_fifo));
    return FifoPop(&serial_rx_fifo);
}

static int uart2_in_waiting(void)
{
    return FifoQueryPushed(&serial_rx_fifo);
}

void UART2_INTERRUPT(void)
{
    int data;
    
    if(LL_USART_IsActiveFlag_RXNE(UART2_INSTANCE))
    {
        data = LL_USART_ReceiveData8(UART2_INSTANCE);
        if(FifoIsFull(&serial_rx_fifo)!=1)
            FifoPush(&serial_rx_fifo, data); 
    }
}

