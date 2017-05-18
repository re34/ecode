#include "bsp_uart1.h"


static uart1_irq_t bsp_uart1_irq = {0};

int bsp_uart1_init(uint32_t baudrate)
{
  /* (1) Enable GPIO clock and configures the USART pins *********************/

  /* Enable the peripheral clock of GPIO Port */
  USART1_GPIO_CLK_ENABLE();

  /* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(USART1_TX_GPIO_PORT, USART1_TX_PIN, LL_GPIO_MODE_ALTERNATE);
  USART1_SET_TX_GPIO_AF();
  LL_GPIO_SetPinSpeed(USART1_TX_GPIO_PORT, USART1_TX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(USART1_TX_GPIO_PORT, USART1_TX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(USART1_TX_GPIO_PORT, USART1_TX_PIN, LL_GPIO_PULL_UP);

  /* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
  LL_GPIO_SetPinMode(USART1_RX_GPIO_PORT, USART1_RX_PIN, LL_GPIO_MODE_ALTERNATE);
  USART1_SET_RX_GPIO_AF();
  LL_GPIO_SetPinSpeed(USART1_RX_GPIO_PORT, USART1_RX_PIN, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetPinOutputType(USART1_RX_GPIO_PORT, USART1_RX_PIN, LL_GPIO_OUTPUT_PUSHPULL);
  LL_GPIO_SetPinPull(USART1_RX_GPIO_PORT, USART1_RX_PIN, LL_GPIO_PULL_UP);

  NVIC_SetPriority(USART1_IRQ_NUM, 0);
  NVIC_EnableIRQ(USART1_IRQ_NUM);
  /* (2) Enable USART peripheral clock and clock source ***********************/
  USART1_CLK_ENABLE();

  /* Set clock source */
  USART1_CLK_SOURCE();

  /* (3) Configure USART functional parameters ********************************/
  
  /* Disable USART prior modifying configuration registers */
  /* Note: Commented as corresponding to Reset value */
  // LL_USART_Disable(USART1_INSTANCE);

  /* TX/RX direction */
  LL_USART_SetTransferDirection(USART1, LL_USART_DIRECTION_TX_RX);

  /* 8 data bit, 1 start bit, 1 stop bit, no parity */
  LL_USART_ConfigCharacter(USART1, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);

  /* No Hardware Flow control */
  /* Reset value is LL_USART_HWCONTROL_NONE */
   LL_USART_SetHWFlowCtrl(USART1, LL_USART_HWCONTROL_NONE);

  /* Oversampling by 16 */
  /* Reset value is LL_USART_OVERSAMPLING_16 */
  // LL_USART_SetOverSampling(USART1_INSTANCE, LL_USART_OVERSAMPLING_16);

  /* Set Baudrate to 115200 using APB frequency set to 80000000 Hz */
  /* Frequency available for USART peripheral can also be calculated through LL RCC macro */
  /* Ex :
      Periphclk = LL_RCC_GetUSARTClockFreq(Instance); or LL_RCC_Getuart1ClockFreq(Instance); depending on USART/uart1 instance
  
      In this example, Peripheral Clock is expected to be equal to 80000000 Hz => equal to SystemCoreClock
  */
  LL_USART_SetBaudRate(USART1, SystemCoreClock/2, LL_USART_OVERSAMPLING_16, 115200); 

  /* (4) Enable USART *********************************************************/
  LL_USART_Enable(USART1);
  
  /* Polling USART initialisation */
  while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))))
  { 
  } 

  LL_USART_EnableIT_RXNE(USART1);
  LL_USART_ClearFlag_TC(USART1);

  return 0;
}

int bsp_uart1_getbyte(void)
{
    return LL_USART_ReceiveData8(USART1);
}

int bsp_uart1_writebyte(uint8_t byte)
{
    while(!LL_USART_IsActiveFlag_TXE(USART1));
    
    LL_USART_TransmitData8(USART1, byte);

    return 0;
}

void bsp_uart1_register_irq(uart1_irq_t irq)
{
    bsp_uart1_irq = irq;
}

void USART1_IRQHandler(void)
{
    if(LL_USART_IsActiveFlag_RXNE(USART1))
    {
        if(bsp_uart1_irq.uart1_rxne_callback!=NULL)
            bsp_uart1_irq.uart1_rxne_callback();
    }

}

