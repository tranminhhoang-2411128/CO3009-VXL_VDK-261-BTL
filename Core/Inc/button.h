/*
 * button.h
 *
 *  Created on: Sep 25, 2026
 *      Author: Hoang
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#include "global.h"

#define NUMBER_OF_BUTTONS 4

#define BTN_1_GPIO_Port D7_GPIO_Port
#define BTN_1_Pin D7_Pin
#define BTN_2_GPIO_Port D8_GPIO_Port
#define BTN_2_Pin D8_Pin
#define BTN_3_GPIO_Port D9_GPIO_Port
#define BTN_3_Pin D9_Pin
#define BTN_4_GPIO_Port D10_GPIO_Port
#define BTN_4_Pin D10_Pin

#define ACTIVE_STATE GPIO_PIN_RESET
#define NORMAL_STATE GPIO_PIN_SET

#define TIMER_HOLD_CHECK 200

extern volatile int button_flag[];
extern volatile int hold_flag[];

void btnInit();
void getInput();

#endif /* INC_BUTTON_H_ */
