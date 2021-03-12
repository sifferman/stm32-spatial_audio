
#ifndef __ETHAN_ERROR_H
#define __ETHAN_ERROR_H

#include <stdint.h>
#include "lcd.h"

void ERROR( uint8_t status ) {
  uint8_t LCD_MESSAGE[7] = "ER ";
  LCD_Clear();
  LCD_MESSAGE[3] = '0' + (status / 100);
  LCD_MESSAGE[4] = '0' + (status % 100) / 10;
  LCD_MESSAGE[5] = '0' + (status % 10);
  LCD_MESSAGE[6] = 0;
  LCD_DisplayString( LCD_MESSAGE );
	while(1);
}

#endif
