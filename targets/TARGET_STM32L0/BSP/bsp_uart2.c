#include "bsp_UART2.h"


static UART2_irq_t bsp_UART2_irq = {0};

int bsp_UART2_init(uint32_t baudrate)
{
  /* (1) Enable GPIO clock and configures the USART pins *********************/

  /* Enable the peripheral clock of GPIO Port */
  UART2_GPIO_CLK_ENABLE();

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(UART2_TX_GPIO_PORT, UART2_TX_PIN, LL_GPIO_MODE_ALTERNATE);
  UART2_SET_TX_GPIO_AF();
  LL_GPIO_SetPinSpeed(UART2_TX_GPIO_PORT, UART2_TX_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);
  LL_GPIO_SetPinOutputType(UART2_TX_GPIO_PORT, UART2_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(UART2_TX_GPIO_PORT, UART2_TX_PIN, LL_GPIO_PULL_UP);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(UART2_RX_GPIO_PORT, UART2_RX_PIN, LL_GPIO_MODE_ALTERNATE);
  UART2_SET_RX_GPIO_AF();
  LL_GPIO_SetPinSpeed(UART2_RX_GPIO_PORT, UART2_RX_PIN, LL_GPIO_SPEED_FREQ_VERY_HIGH);
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

  return 0;
}

int bsp_UART2_getbyte(void)
{
    return LL_USART_ReceiveData8(UART2_INSTANCE);
}

int bsp_UART2_writebyte(uint8_t byte)
{
    while(!LL_USART_IsActiveFlag_TXE(UART2_INSTANCE));
    
    LL_USART_TransmitData8(UART2_INSTANCE, byte);

    return 0;
}

void bsp_UART2_register_irq(UART2_irq_t irq)
{
    bsp_UART2_irq = irq;
}

void UART2_INTERRUPT(void)
{
    if(LL_USART_IsActiveFlag_RXNE(UART2_INSTANCE))
    {
        if(bsp_UART2_irq.UART2_rxne_callback!=NULL)
            bsp_UART2_irq.UART2_rxne_callback();
    }

}

