#include "bsp_uart.h"


static uart_irq_t bsp_uart_irq = {0};

int bsp_uart_init(uint32_t baudrate)
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

  return 0;
}

int bsp_uart_getbyte(void)
{
    return LL_USART_ReceiveData8(UART_INSTANCE);
}

int bsp_uart_writebyte(uint8_t byte)
{
    while(!LL_USART_IsActiveFlag_TXE(UART_INSTANCE));
    
    LL_USART_TransmitData8(UART_INSTANCE, byte);

    return 0;
}

void bsp_uart_register_irq(uart_irq_t irq)
{
    bsp_uart_irq = irq;
}

void UART_INTERRUPT(void)
{
    if(LL_USART_IsActiveFlag_RXNE(UART_INSTANCE))
    {
        if(bsp_uart_irq.uart_rxne_callback!=NULL)
            bsp_uart_irq.uart_rxne_callback();
    }

}

