/*
 * ECE 153B - Winter 2021
 *
 * Name     : Ethan Sifferman
 * Section  : Wednesday
 * Lab      : 4A
 */

#include "stm32l476xx.h"

#include "LED.h"
#include "LCD.h"
#include "SysClock.h"
#include "UART.h"

#include "sample_buffer.h"

#include "i2c.h"
#include "cs43l22.h"
#include "SAI.h"

#include <string.h>
#include <stdio.h>

#include "part.h"












// Initializes USART for each part of Lab 3A
// part = 1: UART Communication with Termite
// part = 2: Bluetooth Communication with Phone
void Init_USARTx( int8_t part ) {
	if ( part == 1 ) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init( USART2 );
	} else if( part == 2 ) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init( USART1 );
	} else {
		// Do nothing...
	}
}


int main(void) {
  
  System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
  LCD_Initialization();
  
  buffer_q_init();
	
	// Initialize UART -- change the argument depending on the part you are working on
	Init_USARTx( _PART );

  
  i2c_init( I2C1 );

	SAI_Init();

	/* Set up cs43l22 */
	cs43l22_init();
  
  // Init cs43l22 over i2c
	cs43l22_play();
  
  // wait for main buffer to fill
  while ( !buffer_q_almostfull() );

  const sample_t * current_sample;
  sample_t previous_sample = {0,0};
  uint8_t side = 'L';
  // start sending samples
  while ( 1 ) if (
    ( SAI1_Block_A->SR & SAI_xSR_FLVL) != SAI_xSR_FLVL
      && (SAI1_Block_A->SR & SAI_xSR_FREQ)
    ) {
    switch ( side ) {

      case 'L':
        current_sample = buffer_q_get();
        if ( current_sample ) {
          SAI1_Block_A->DR = ((uint16_t)current_sample->left)<<8;
          previous_sample.left = current_sample->left;
        } else {
          SAI1_Block_A->DR = ((uint16_t)previous_sample.left)<<8;
          SAI1_Block_A->DR = 0;
        }
        side = 'R';
        break;

      case 'R':
        if ( current_sample ) {
          SAI1_Block_A->DR = ((uint16_t)current_sample->right)<<8;
          previous_sample.right = current_sample->right;
        } else {
          SAI1_Block_A->DR = ((uint16_t)previous_sample.right)<<8;
          SAI1_Block_A->DR = 0;
        }
        buffer_q_pop();
        side = 'L';
        break;

      default: side = 'L'; break;
    }
    
    USART2->TDR = buffer_q_almostfull() ? 'B' : buffer_q_empty() ? 'E' : 'g';
	}
  

}
