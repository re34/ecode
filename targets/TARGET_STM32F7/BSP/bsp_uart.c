#include "bsp_uart.h"


static uart_irq_t bsp_uart_irq = {0};

int bsp_uart_init(uint32_t baudrate)
{
  /* (1) Enable GPIO clock and configures the USART pins *********************/

  /* Enable the peripheral clock of GPIO Port */
  USART3_GPIO_CLK_ENABLE();

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(USART3_TX_GPIO_PORT, USART3_TX_PIN, LL_GPIO_MODE_ALTERNATE);
  USART3_SET_TX_GPIO_AF();
  LL_GPIO_SetPinSpeed(USART3_TX_GPIO_PORT, USART3_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(USART3_TX_GPIO_PORT, USART3_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(USART3_TX_GPIO_PORT, USART3_TX_PIN, LL_GPIO_PULL_UP);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(USART3_RX_GPIO_PORT, USART3_RX_PIN, LL_GPIO_MODE_ALTERNATE);
  USART3_SET_RX_GPIO_AF();
  LL_GPIO_SetPinSpeed(USART3_RX_GPIO_PORT, USART3_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(USART3_RX_GPIO_PORT, USART3_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(USART3_RX_GPIO_PORT, USART3_RX_PIN, LL_GPIO_PULL_UP);

  NVIC_SetPriority(USART3_IRQ_NUM, 0);
  NVIC_EnableIRQ(USART3_IRQ_NUM);
  /* (2) Enable USART peripheral clock and clock source ***********************/
  USART3_CLK_ENABLE();

  /* Set clock source */
  USART3_CLK_SOURCE();

  /* (3) Configure USART functional parameters ********************************/
  
  /* Disable USART prior modifying configuration registers */
  /* Note: Commented as corresponding to Reset value */
  // LL_USART_Disable(USART3_INSTANCE);

  /* TX/RX direction */
  LL_USART_SetTransferDirection(USART3, LL_USART_DIRECTION_TX_RX);

  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(USART3, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

  /* No Hardware Flow control */
  /* Reset value is LL_USART_HWCONTROL_NONE */
   LL_USART_SetHWFlowCtrl(USART3, LL_USART_HWCONTROL_NONE);

  /* Oversampling by 16 */
  /* Reset value is LL_USART_OVERSAMPLING_16 */
  // LL_USART_SetOverSampling(USART3_INSTANCE, LL_USART_OVERSAMPLING_16);

  /* Set Baudrate to 115200 using APB frequency set to 80000000 Hz */
  /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
  /* Ex :
      Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_GetUARTClockFreq(Instance); depending on USART/UART instance
  
      In this example, Peripheral Clock is expected to be equal to 80000000 Hz => equal to SystemCoreClock
  */
  LL_USART_SetBaudRate(USART3, SystemCoreClock/4, LL_USART_OVERSAMPLING_16, baudrate); 

  /* (4) Enable USART *********************************************************/
  LL_USART_Enable(USART3);
  

  LL_USART_EnableIT_RXNE(USART3);
  LL_USART_ClearFlag_TC(USART3);

  return 0;
}

int bsp_uart_getbyte(void)
{
    return LL_USART_ReceiveData8(USART3);
}

int bsp_uart_writebyte(uint8_t byte)
{
    while(!LL_USART_IsActiveFlag_TXE(USART3));
    
    LL_USART_TransmitData8(USART3, byte);

    return 0;
}

void bsp_uart_register_irq(uart_irq_t irq)
{
    bsp_uart_irq = irq;
}

void USART3_IRQHandler(void)
{
    if(LL_USART_IsActiveFlag_RXNE(USART3))
    {
        if(bsp_uart_irq.uart_rxne_callback!=NULL)
            bsp_uart_irq.uart_rxne_callback();
    }

}
