/*
 * ECE 153B - Winter 2021
 *
 * Name     : Ethan Sifferman
 * Section  : Wednesday
 * Lab      : 4A
 */

#include "UART.h"
#include "sample_buffer.h"


void USART_Init( USART_TypeDef * USARTx ) {
  // Disable USART before modifying USARTx registers
  USARTx  -> CR1      &= ~USART_CR1_UE;

  // Set world length to 8 bits, set oversampling mode to oversampling by 16, and set the number of stop bits to 1.
  USARTx  -> CR1      &= ~USART_CR1_M;
  USARTx  -> CR1      &= ~USART_CR1_OVER8;
  USARTx  -> CR2      &= ~USART_CR2_STOP;

  // Set the baud rate to 9600.
  USARTx  -> BRR       = 80000000 / 1411200;

  // In the control registers, enable both the transmitter and receiver
  USARTx  -> CR1      |=  USART_CR1_TE;
  USARTx  -> CR1      |=  USART_CR1_RE;
  
  // Enable Interrupt
  USARTx  -> CR1      |=  USART_CR1_RXNEIE;
  
  // Configure and Enable in NVIC
  if ( USARTx == USART1 ) {
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 0);
  }
  else if ( USARTx == USART2 ) {
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_SetPriority(USART2_IRQn, 0);
  } else {
    /* Do nothing */
  }

  // Now that everything has been set up, enable USART in the control registers.
  USARTx  -> CR1      |=  USART_CR1_UE;
}





/* Part 1 */

void UART2_Init(void) {
  // Enable the USART2 clock in the peripheral clock register
  RCC     -> APB1ENR1 |=  RCC_APB1ENR1_USART2EN;

  // Select the system clock as the USART2 clock source in the peripherals independent
  // clock configuration register
  RCC     -> CFGR     &= ~RCC_CFGR_MCOSEL;
  RCC     -> CFGR     |=  RCC_CFGR_MCOSEL_0;
}

void UART2_GPIO_Init(void) {
  // Enable the GPIO clock
  RCC     -> AHB2ENR  |=  RCC_AHB2ENR_GPIODEN;

  // Set PD5 mode to alternative function
  GPIOD   -> MODER    &= ~GPIO_MODER_MODE5_0;
  GPIOD   -> MODER    |=  GPIO_MODER_MODE5_1;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL5_0;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL5_1;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL5_2;
  GPIOD   -> AFR[0]   &= ~GPIO_AFRL_AFSEL5_3;

  // Set PD6 mode to alternative function
  GPIOD   -> MODER    &= ~GPIO_MODER_MODE6_0;
  GPIOD   -> MODER    |=  GPIO_MODER_MODE6_1;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL6_0;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL6_1;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL6_2;
  GPIOD   -> AFR[0]   &= ~GPIO_AFRL_AFSEL6_3;

  // Set both pins to very high speed
  GPIOD   -> OSPEEDR  |=  GPIO_OSPEEDR_OSPEED5;
  GPIOD   -> OSPEEDR  |=  GPIO_OSPEEDR_OSPEED6;

  // Set both pins to push-pull output type
  GPIOD   -> OTYPER   &= ~GPIO_OTYPER_OT5;
  GPIOD   -> OTYPER   &= ~GPIO_OTYPER_OT6;

  // Configure both pins to use pull-up resistors
  GPIOD   -> PUPDR    &= ~GPIO_PUPDR_PUPD5;
  GPIOD   -> PUPDR    &= ~GPIO_PUPDR_PUPD6;
  GPIOD   -> PUPDR    |=  GPIO_PUPDR_PUPD5_0;
  GPIOD   -> PUPDR    |=  GPIO_PUPDR_PUPD6_0;
}



/* Part 2 */

void UART1_Init(void) {
  // Enable the USART1 clock in the peripheral clock register.
  RCC     -> APB2ENR  |=  RCC_APB2ENR_USART1EN;

  // Select the system clock as the USART1 clock source in the peripherals independent
  // clock configuration register
  RCC     -> CFGR     &= ~RCC_CFGR_MCOSEL;
  RCC     -> CFGR     |=  RCC_CFGR_MCOSEL_0;
}

void UART1_GPIO_Init(void) {
  // Enable the GPIO clock in RCC
  RCC     -> AHB2ENR  |=  RCC_AHB2ENR_GPIOBEN;

  // Set PB6 mode to alternative function
  GPIOB   -> MODER    &= ~GPIO_MODER_MODE6_0;
  GPIOB   -> MODER    |=  GPIO_MODER_MODE6_1;
  GPIOB   -> AFR[0]   |=  GPIO_AFRL_AFSEL6_0;
  GPIOB   -> AFR[0]   |=  GPIO_AFRL_AFSEL6_1;
  GPIOB   -> AFR[0]   |=  GPIO_AFRL_AFSEL6_2;
  GPIOB   -> AFR[0]   &= ~GPIO_AFRL_AFSEL6_3;

  // Set PB7 mode to alternative function
  GPIOB   -> MODER    &= ~GPIO_MODER_MODE7_0;
  GPIOB   -> MODER    |=  GPIO_MODER_MODE7_1;
  GPIOB   -> AFR[0]   |=  GPIO_AFRL_AFSEL7_0;
  GPIOB   -> AFR[0]   |=  GPIO_AFRL_AFSEL7_1;
  GPIOB   -> AFR[0]   |=  GPIO_AFRL_AFSEL7_2;
  GPIOB   -> AFR[0]   &= ~GPIO_AFRL_AFSEL7_3;

  // Set both pins to very high speed
  GPIOB   -> OSPEEDR  |=  GPIO_OSPEEDR_OSPEED6;
  GPIOB   -> OSPEEDR  |=  GPIO_OSPEEDR_OSPEED7;

  // Set both pins to have a push-pull output type.
  GPIOB 	-> OTYPER   &= ~GPIO_OTYPER_OT6;
  GPIOB 	-> OTYPER   &= ~GPIO_OTYPER_OT7;

  // Configure both pins to use pull-up resistors
  GPIOB 	-> PUPDR    &= ~GPIO_PUPDR_PUPD6;
  GPIOB 	-> PUPDR    &= ~GPIO_PUPDR_PUPD7;
  GPIOB 	-> PUPDR    |=  GPIO_PUPDR_PUPD6_0;
  GPIOB 	-> PUPDR    |=  GPIO_PUPDR_PUPD7_0;
}





/* Read / Write */

typedef enum STATE {
  left_l, left_u,
  right_l, right_u
} state_t;

volatile state_t state = left_l;
volatile uint16_t left_sample, right_sample;

void handle_RXNE( USART_TypeDef * USARTx ) {
  // little endian
  uint8_t RDR = ( USARTx->RDR & 0xFF );
  switch ( state ) {
    case left_l:
      left_sample = 0;
      left_sample |= RDR;
      state = left_u;
      break;
    case left_u:
      left_sample |= (RDR<<8);
      state = right_l;
      break;
    case right_l:
      right_sample = 0;
      right_sample |= RDR;
      state = right_u;
      break;
    case right_u:
      right_sample |= (RDR<<8);
      state = left_l;
      buffer_q_push( left_sample, right_sample );
      // if TX is empty
      //   send either "BAD" or "good"
      if ( USARTx->ISR & USART_ISR_TXE )
        USARTx->TDR = buffer_q_almostfull() ? 'B' : 'g';
      break;
    default: break;
  }

}
void USART1_IRQHandler(void) {
  if ( USART1->ISR & USART_ISR_RXNE ) {
    handle_RXNE(USART1);
    USART1->ISR &= ~USART_ISR_RXNE;
  }
}
void USART2_IRQHandler(void) {
  if ( USART2->ISR & USART_ISR_RXNE ) {
    handle_RXNE(USART2);
    USART2->ISR &= ~USART_ISR_RXNE;
  }
}

uint8_t USART_Read (USART_TypeDef * USARTx) {
	// SR_RXNE (Read data register not empty) bit is set by hardware
	while (!(USARTx->ISR & USART_ISR_RXNE));  // Wait until RXNE (RX not empty) bit is set
	// USART resets the RXNE flag automatically after reading DR
	return ((uint8_t)(USARTx->RDR & 0xFF));
	// Reading USART_DR automatically clears the RXNE flag 
}

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes) {
	int i;
	// TXE is cleared by a write to the USART_DR register.
	// TXE is set by hardware when the content of the TDR 
	// register has been transferred into the shift register.
	for (i = 0; i < nBytes; i++) {
		while (!(USARTx->ISR & USART_ISR_TXE));   	// wait until TXE (TX empty) bit is set
		// Writing USART_DR automatically clears the TXE flag 	
		USARTx->TDR = buffer[i] & 0xFF;
		USART_Delay(300);
	}
	while (!(USARTx->ISR & USART_ISR_TC));   		  // wait until TC bit is set
	USARTx->ISR &= ~USART_ISR_TC;
}

void USART_Delay(uint32_t us) {
	uint32_t time = 100*us/7;
	while(--time);
}
