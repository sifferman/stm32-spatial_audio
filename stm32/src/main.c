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


	while(1) {
    
	}
}
