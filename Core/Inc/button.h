/*
 * button.h
 *
 *  Created on: Sep 25, 2026
 *      Author: Hoang
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "main.h"

#define NUMBER_OF_BUTTONS 3

#define ACTIVE_STATE GPIO_PIN_RESET
#define NORMAL_STATE GPIO_PIN_SET

#define BTN_PORT GPIOA
#define BTN_0_PIN GPIO_PIN_6
#define BTN_1_PIN GPIO_PIN_7
#define BTN_2_PIN GPIO_PIN_8

#define TIMER_HOLD_CHECK 200

extern int button_flag[];
extern int hold_flag[];

void getInput();

#endif /* INC_BUTTON_H_ */
