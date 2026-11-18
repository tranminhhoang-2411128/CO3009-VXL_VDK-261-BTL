/*
 * lcd.h
 *
 *  Created on: Nov 9, 2026
 *      Author: Hoang
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "global.h"
#define  LCD_ADDR 0x21

void LCD_Connect();
void LCD_Init();
void LCD_SetBacklight(int status);
void LCD_ClearDisplay();
//void LCD_Print (char* str, int row) (this is LCD 16x2)

#endif /* INC_LCD_H_ */
