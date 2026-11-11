/*
 * button.c
 *
 *  Created on: Sep 25, 2026
 *      Author: Hoang
 */

#include "button.h"

int input0[NUMBER_OF_BUTTONS];
int input1[NUMBER_OF_BUTTONS];
int input2[NUMBER_OF_BUTTONS];
int input3[NUMBER_OF_BUTTONS];

static GPIO_TypeDef* button_port[NUMBER_OF_BUTTONS] = {BTN_1_GPIO_Port, BTN_2_GPIO_Port
															, BTN_3_GPIO_Port, BTN_4_GPIO_Port};
static const int button_pin[NUMBER_OF_BUTTONS] = {BTN_1_Pin, BTN_2_Pin, BTN_3_Pin, BTN_4_Pin};

int timer_hold[NUMBER_OF_BUTTONS];
int current_state[NUMBER_OF_BUTTONS];
int button_flag[NUMBER_OF_BUTTONS];
int hold_flag[NUMBER_OF_BUTTONS];

void btnInit(){
	for (int i = 0; i < NUMBER_OF_BUTTONS; i++){
		input0[i] = NORMAL_STATE;
		input1[i] = NORMAL_STATE;
		input2[i] = NORMAL_STATE;
		input3[i] = NORMAL_STATE;

		timer_hold[i] = TIMER_HOLD_CHECK;
		current_state[i] = 0;
		button_flag[i] = 0;
		hold_flag[i] = 0;
	}
}


/*
 * current_state (input2,input3)
 * 0: initial + released (1,1); (1,0)
 * 1: pressed (0,1)
 * 2: holding (0,0)
 * 3: execute holding process
 */

void getInput(){
	for (int i = 0; i < NUMBER_OF_BUTTONS; i++){
		input0[i] = input1[i];
		input1[i] = input2[i];
		input2[i] = HAL_GPIO_ReadPin(button_port[i], button_pin[i]);

		if ((current_state[i] == 0) && (input2[i] == ACTIVE_STATE)){
			current_state[i] = 1;
		}

		if ((input0[i] == input1[i]) && (input1[i] == input2[i])){ //valid signal
			switch (current_state[i]){
			case 0:
				input3[i] = input2[i];
				timer_hold[i] = TIMER_HOLD_CHECK;
				break;
			case 1:
				input3[i] = input2[i];
				button_flag[i] = 1;
				current_state[i] = 2;
				break;
			case 2:
				timer_hold[i]--;
				current_state[i] = (timer_hold[i] <= 0) ? 3 : 2;
				break;
			case 3:
				hold_flag[i] = 1;
				timer_hold[i] = TIMER_HOLD_CHECK;
				current_state[i] = 2;
				break;
			}
		}
		else {
			current_state[i] = 0;
			hold_flag[i] = 0;
		}
	}
}

int isButtonPressed(int i){
	if (button_flag[i] == 1)
	{
		button_flag[i] = 0;
		return 1;
	}
	else {
		return 0;
	}
}

int isButtonHeld(int i){
	return hold_flag[i]; //Only lowers the flag when the button is let go
}
