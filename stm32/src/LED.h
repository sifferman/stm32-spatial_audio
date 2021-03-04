/*
 * ECE 153B - Winter 2021
 *
 * Name     : Ethan Sifferman
 * Section  : Wednesday
 * Lab      : 4A
 */

#ifndef __STM32L476G_DISCOVERY_LED_H
#define __STM32L476G_DISCOVERY_LED_H

#include "stm32l476xx.h"

void LED_Init(void);

void Red_LED_Off(void);
void Red_LED_On(void);
void Red_LED_Toggle(void);

void Green_LED_Off(void);
void Green_LED_On(void);
void Green_LED_Toggle(void);

#endif
