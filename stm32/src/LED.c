/*
 * ECE 153B - Winter 2021
 *
 * Name     : Ethan Sifferman
 * Section  : Wednesday
 * Lab      : 4A
 */

#include "LED.h"

void LED_Init(void) {

  // Enable LED GPIO Clocks
  RCC->AHB2ENR  |=  (RCC_AHB2ENR_GPIOBEN
                    |RCC_AHB2ENR_GPIOEEN)
  ;

  // Pin Initialization for Red LED (PB2)
  GPIOB->MODER  &= ~GPIO_MODER_MODE2  ;
  GPIOB->MODER  |=  GPIO_MODER_MODE2_0; // Output

  GPIOB->OTYPER &= ~GPIO_OTYPER_OT2   ; // Push-Pull

  GPIOB->PUPDR  &= ~GPIO_PUPDR_PUPD2  ;
                                        // No Pull-Up, No Pull-Down

  GPIOB->ODR    &= ~GPIO_ODR_OD2      ; // Turn off LED


  // Pin Initialization for Green LED (PE8)
  GPIOE->MODER  &= ~GPIO_MODER_MODE8  ;
  GPIOE->MODER  |=  GPIO_MODER_MODE8_0; // Output

  GPIOE->OTYPER &= ~GPIO_OTYPER_OT8   ; // Push-Pull

  GPIOE->PUPDR  &= ~GPIO_PUPDR_PUPD8  ;
                                        // No Pull-Up, No Pull-Down

  GPIOE->ODR    &= ~GPIO_ODR_OD8      ; // Turn off LED
}

void Red_LED_Off(void) {
  GPIOB->ODR &= ~GPIO_ODR_OD2;
}

void Red_LED_On(void) {
  GPIOB->ODR |=  GPIO_ODR_OD2;
}

void Red_LED_Toggle(void){
  GPIOB->ODR ^=  GPIO_ODR_OD2;
}

void Green_LED_Off(void) {
  GPIOE->ODR &= ~GPIO_ODR_OD8;
}

void Green_LED_On(void) {
  GPIOE->ODR |=  GPIO_ODR_OD8;
}

void Green_LED_Toggle(void) {
  GPIOE->ODR ^=  GPIO_ODR_OD8;
}
