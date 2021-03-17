/*
 * ECE 153B - Winter 2021
 *
 * Name     : Ethan Sifferman
 * Section  : Wednesday
 * Lab      : 4A
 */

#include "UART.h"
#include <stdio.h>

#include "part.h"

// Implement a dummy __FILE struct, which is called with the FILE structure.
struct __FILE {
	int dummy;
};

// We have to define FILE if prinf is used
FILE __stdout;
FILE __stdin;
 
// Retarget printf() to USART1/USART2
int fputc( int ch, FILE * f ) { 
	uint8_t c;
	c = ch & 0x00FF;
	switch ( _PART ) {
		case 1: USART_Write(USART2, (uint8_t *)&c, 1);
			break;
		case 2: USART_Write(USART1, (uint8_t *)&c, 1);
			break;
		default: // Do nothing
			break;
	}
	return(ch);
}

// Retarget scanf() to USART1/USART2
int fgetc( FILE * f ) {  
	uint8_t rxByte;
		switch ( _PART ) {
		case 1: rxByte = USART_Read(USART2);
			break;
		case 2: rxByte = USART_Read(USART1);
			break;
		default: // Do nothing
			break;
	}
	return rxByte;
}
