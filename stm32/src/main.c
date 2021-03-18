/*
 * ECE 153B - Winter 2021
 *
 * Name     : Ethan Sifferman
 * Section  : Wednesday
 * Lab      : 4A
 */

#include "stm32l476xx.h"

#include "L3GD20.h"
#include "LED.h"
#include "LCD.h"
#include "SysClock.h"
#include "UART.h"
#include "SysTimer.h"

#include "sample_buffer.h"

#include "i2c.h"
#include "cs43l22.h"
#include "SAI.h"

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "part.h"



double Z;

void L3GD20_measure( const uint8_t bit, const uint8_t upper, const uint8_t lower ) {
  uint8_t buffer = 0;
  int16_t measurement = 0;

  GYRO_IO_Read( &buffer, L3GD20_STATUS_REG_ADDR, sizeof(buffer) );

  // New data is available
  if ( buffer & ( 1 << bit ) ) {
    // read upper
    buffer = 0;
    GYRO_IO_Read( &buffer, upper, sizeof(buffer) );
    measurement |= buffer << 8;

    // read lower
    buffer = 0;
    GYRO_IO_Read( &buffer, lower, sizeof(buffer) );
    measurement |= buffer;

  }

  // Convert to DPS
  Z += (measurement * 0.07) / 636.62;
  while ( Z < 0 ) {
    Z += 6.28;
  }
  while ( Z > 6.28 ) {
    Z -= 6.27;
  }
  //Z = 0;
}




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
  
  Z = 0;
  
  System_Clock_Init(); // Switch System Clock = 80 MHz
  SysTick_Init();
  LED_Init();
  LCD_Initialization();
  
  // Initialize Gyroscope
  GYRO_Init();
  
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
  //while ( !buffer_q_almostfull() );

  const sample_t * current_sample;
  sample_t previous_sample = {0,0};
  uint8_t side = 'L';
  uint16_t data_left, data_right;
  // start sending samples
  while ( 1 ) {
    L3GD20_measure( 2, L3GD20_OUT_Z_H_ADDR, L3GD20_OUT_Z_L_ADDR );
    if (
    ( SAI1_Block_A->SR & SAI_xSR_FLVL) != SAI_xSR_FLVL
      && (SAI1_Block_A->SR & SAI_xSR_FREQ)
    ) {
      switch ( side ) {

        case 'L':
          current_sample = buffer_q_get();
          if ( current_sample ) {
            data_left = (((uint16_t)current_sample->left)<<7);
            SAI1_Block_A->DR = data_left;
            previous_sample.left = current_sample->left;
          } else {
            data_left = (((uint16_t)previous_sample.left)<<7);
            SAI1_Block_A->DR = data_left;
            SAI1_Block_A->DR = 0;
          }
          side = 'R';
          break;

        case 'R':
          if ( current_sample ) {
            data_right = (((uint16_t)current_sample->right)<<7);
            SAI1_Block_A->DR = data_right;
            previous_sample.right = current_sample->right;
          } else {
            data_right = (((uint16_t)previous_sample.right)<<7);
            SAI1_Block_A->DR = data_right;
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
  

}
