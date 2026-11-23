/*
 * fsm_lcd.c
 *
 *  Created on: Nov 23, 2026
 *      Author: Hoang
 */

#include "fsm_lcd.h"

void fsm_lcd_init(){
	setTimer(1, LCD_INIT_TIME);
}

void fsm_lcd_run(){
	if (timer_flag[1] == 1){
		switch (LCD_flag) {
			case 0:
				LCD_Connect();
				setTimer(1, LCD_STEP_TIME);
				break;

			case 1:
				LCD_Init();
				setTimer(1, LCD_STEP_TIME);
				break;

			case 2:
				LCD_Print(first_row_buffer, 0);
				setTimer(1, LCD_REFRESH_PERIOD);
				LCD_flag = 3;
				break;


			case 3:
				LCD_Print(second_row_buffer, 1);
				setTimer(1, LCD_REFRESH_PERIOD);
				LCD_flag = 2;
				break;

			default:
				break;
		}
	}
}
