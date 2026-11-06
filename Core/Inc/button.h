/*
 * button.h
 *
 *  Created on: Sep 25, 2026
 *      Author: Hoang
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#define NUMBER_OF_BUTTONS 4

#define ACTIVE_STATE GPIO_PIN_RESET
#define NORMAL_STATE GPIO_PIN_SET

#define BTN_0_Pin GPIO_PIN_6
#define BTN_1_Pin GPIO_PIN_7
#define BTN_2_Pin GPIO_PIN_8
#define BTN_3_Pin GPIO_PIN_9

#define TIMER_HOLD_CHECK 200

extern int button_flag[];
extern int hold_flag[];

void getInput();

#endif /* INC_BUTTON_H_ */
