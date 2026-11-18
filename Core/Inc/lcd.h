/*
 * lcd.h
 *
 *  Created on: Nov 9, 2026
 *      Author: Hoang
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_

#include "global.h"
#define LCD_ADDR 0x21
#define LCD_COLS 16
#define LCD_CALIB_TIME 20
#define LCD_INIT_TIME  60     /* ms, power-on wait (> 40 ms @ 2.7 V)            */
#define LCD_STEP_TIME  20     /* ms, gap between init steps (> 4.1 ms)          */

void LCD_Connect();
void LCD_Init();
void LCD_SetBacklight(int status);
void LCD_ClearDisplay();
void LCD_Print(char* str, int row);     /* this is LCD 16x2 */

/* LCD_flag
 * 0: not connected
 * 1: connected, initializing
 * 2: ready
 * 3: printing second row
 */
extern int LCD_flag;

#endif /* INC_LCD_H_ */
