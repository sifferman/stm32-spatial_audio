/*
 * ECE 153B - Winter 2021
 *
 * Name     : Ethan Sifferman
 * Section  : Wednesday
 * Lab      : 4A
 */

#ifndef __STM32L476G_DISCOVERY_UART_H
#define __STM32L476G_DISCOVERY_UART_H

#include "stm32l476xx.h"

#define BufferSize 32

void UART1_Init(void);
void UART2_Init(void);
void UART1_GPIO_Init(void);
void UART2_GPIO_Init(void);

void USART_Init(USART_TypeDef* USARTx);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

void USART_Write(USART_TypeDef * USARTx, uint8_t *buffer, uint32_t nBytes);
uint8_t USART_Read(USART_TypeDef * USARTx);
void USART_Delay(uint32_t us);

#endif
