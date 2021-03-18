/*
 * ECE 153B - Winter 2021
 *
 * Name     : Ethan Sifferman
 * Section  : Wednesday
 * Lab      : 4B
 */

#include "SPI.h"
#include "SysTimer.h"

// Note: When the data frame size is 8 bit, "SPIx->DR = byte_data;" works incorrectly.
// It mistakenly send two bytes out because SPIx->DR has 16 bits. To solve the program,
// we should use "*((volatile uint8_t*)&SPIx->DR) = byte_data";

// LSM303C eCompass (a 3D accelerometer and 3D magnetometer module) SPI Interface:
//   MAG_DRDY = PC2   MEMS_SCK  = PD1 (SPI2_SCK)   XL_CS  = PE0
//   MAG_CS   = PC0   MEMS_MOSI = PD4 (SPI2_MOSI)  XL_INT = PE1
//   MAG_INT  = PC1
//
// L3GD20 Gyro (three-axis digital output) SPI Interface:
//   MEMS_SCK  = PD1 (SPI2_SCK)    GYRO_CS   = PD7 (GPIO)
//   MEMS_MOSI = PD4 (SPI2_MOSI)   GYRO_INT1 = PD2
//   MEMS_MISO = PD3 (SPI2_MISO)   GYRO_INT2 = PB8

extern uint8_t Rx1_Counter;
extern uint8_t Rx2_Counter;

void SPI2_GPIO_Init(void) {
  // Enable the clock (in RCC) for the GPIO pin.
  RCC     -> AHB2ENR  |=  RCC_AHB2ENR_GPIODEN;

  // Set PD1 mode to alternative function
  GPIOD   -> MODER    &= ~GPIO_MODER_MODE1_0;
  GPIOD   -> MODER    |=  GPIO_MODER_MODE1_1;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL1_0;
  GPIOD   -> AFR[0]   &= ~GPIO_AFRL_AFSEL1_1;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL1_2;
  GPIOD   -> AFR[0]   &= ~GPIO_AFRL_AFSEL1_3;

  // Set PD3 mode to alternative function
  GPIOD   -> MODER    &= ~GPIO_MODER_MODE3_0;
  GPIOD   -> MODER    |=  GPIO_MODER_MODE3_1;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL3_0;
  GPIOD   -> AFR[0]   &= ~GPIO_AFRL_AFSEL3_1;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL3_2;
  GPIOD   -> AFR[0]   &= ~GPIO_AFRL_AFSEL3_3;

  // Set PD4 mode to alternative function
  GPIOD   -> MODER    &= ~GPIO_MODER_MODE4_0;
  GPIOD   -> MODER    |=  GPIO_MODER_MODE4_1;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL4_0;
  GPIOD   -> AFR[0]   &= ~GPIO_AFRL_AFSEL4_1;
  GPIOD   -> AFR[0]   |=  GPIO_AFRL_AFSEL4_2;
  GPIOD   -> AFR[0]   &= ~GPIO_AFRL_AFSEL4_3;

  // Set pin outputs to push-pull
  GPIOD   -> OTYPER   &= ~GPIO_OTYPER_OT1;
  GPIOD   -> OTYPER   &= ~GPIO_OTYPER_OT3;
  GPIOD   -> OTYPER   &= ~GPIO_OTYPER_OT4;

  // Set GPIO pin speed to very high
  GPIOD   -> OSPEEDR  |=  GPIO_OSPEEDR_OSPEED1;
  GPIOD   -> OSPEEDR  |=  GPIO_OSPEEDR_OSPEED3;
  GPIOD   -> OSPEEDR  |=  GPIO_OSPEEDR_OSPEED4;

  // Configure GPIO pins to no pull-up/down resistors
  GPIOD   -> PUPDR    &= ~GPIO_PUPDR_PUPD1;
  GPIOD   -> PUPDR    &= ~GPIO_PUPDR_PUPD3;
  GPIOD   -> PUPDR    &= ~GPIO_PUPDR_PUPD4;

}

void SPI_Init(void){
  // Enable the clock for SPI2
  RCC     -> APB1ENR1 |=  RCC_APB1ENR1_SPI2EN;

  // Reset SPI2
  RCC     -> APB1RSTR1|=  RCC_APB1RSTR1_SPI2RST;
  RCC     -> APB1RSTR1&= ~RCC_APB1RSTR1_SPI2RST;

  // Disable SPI before modifying SPI2 registers
  SPI2    -> CR1      &= ~SPI_CR1_SPE;

  // Full duplex communication
  SPI2    -> CR1      &= ~SPI_CR1_RXONLY;

  // 2-line unidirectional data mode
  SPI2    -> CR1      &= ~SPI_CR1_BIDIMODE;

  // Disable the output in bidirectional mode
  SPI2    -> CR1      &= ~SPI_CR1_BIDIOE;

  // Frame format set to big endian
  SPI2    -> CR1      &= ~SPI_CR1_LSBFIRST;

  // Data length set to 8 bits
  SPI2    -> CR2      &= ~SPI_CR2_DS;
  SPI2    -> CR2      |=  SPI_CR2_DS_0;
  SPI2    -> CR2      |=  SPI_CR2_DS_1;
  SPI2    -> CR2      |=  SPI_CR2_DS_2;

  // Frame format to SPI Motorola mode
  SPI2    -> CR2      &= ~SPI_CR2_FRF;

  // Set clock polarity to 0
  SPI2    -> CR1      &= ~SPI_CR1_CPOL;

  // Enable data capture edge
  SPI2    -> CR1      &= ~SPI_CR1_CPHA;

  // Baud rate prescaler set to 16
  SPI2    -> CR1      &= ~SPI_CR1_BR;
  SPI2    -> CR1      |=  SPI_CR1_BR_0;
  SPI2    -> CR1      |=  SPI_CR1_BR_1;

  // Disable CRC
  SPI2    -> CR1      &= ~SPI_CR1_CRCEN;

  // Set the board to operate in master mode
  SPI2    -> CR1      |=  SPI_CR1_MSTR;

  // Enable software peripheral management and NSS pulse management
  SPI2    -> CR1      |=  SPI_CR1_SSM;
  SPI2    -> CR2      |=  SPI_CR2_NSSP;

  // Set the internal peripheral select bit
  SPI2    -> CR1      |=  SPI_CR1_SSI;

  // Set the FIFO reception threshold to 1/4 (8 bit)
  SPI2    -> CR2      |=  SPI_CR2_FRXTH;

  // Reenable SPI
  SPI2    -> CR1      |=  SPI_CR1_SPE;

}

void SPI_Write(SPI_TypeDef * SPIx, uint8_t *txBuffer, uint8_t * rxBuffer, int size) {
	volatile uint32_t tmpreg;
	int i = 0;
	for (i = 0; i < size; i++) {
		while( (SPIx->SR & SPI_SR_TXE ) != SPI_SR_TXE );  // Wait for TXE (Transmit buffer empty)
		*((volatile uint8_t*)&SPIx->DR) = txBuffer[i];
		while((SPIx->SR & SPI_SR_RXNE ) != SPI_SR_RXNE); // Wait for RXNE (Receive buffer not empty)
		rxBuffer[i] = *((__IO uint8_t*)&SPIx->DR);
	}
	while( (SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY ); // Wait for BSY flag cleared
}

void SPI_Read(SPI_TypeDef * SPIx, uint8_t *rxBuffer, int size) {
	int i = 0;
	for (i = 0; i < size; i++) {
		while( (SPIx->SR & SPI_SR_TXE ) != SPI_SR_TXE ); // Wait for TXE (Transmit buffer empty)
		*((volatile uint8_t*)&SPIx->DR) = rxBuffer[i];
		// The clock is controlled by master. Thus the master has to send a byte
		// data to the slave to start the clock.
		while((SPIx->SR & SPI_SR_RXNE ) != SPI_SR_RXNE);
		rxBuffer[i] = *((__IO uint8_t*)&SPIx->DR);
	}
	while( (SPIx->SR & SPI_SR_BSY) == SPI_SR_BSY ); // Wait for BSY flag cleared
}

void SPI_Delay(uint32_t us) {
	uint32_t i, j;
	for (i = 0; i < us; i++) {
		for (j = 0; j < 18; j++) // This is an experimental value.
			(void)i;
	}
}
