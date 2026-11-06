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

static const GPIO_TypeDef* button_port[NUMBER_OF_BUTTONS] = {BTN_0_PORT, BTN_1_PORT, BTN_2_PORT, BTN_3_PORT};
static const int button_pin[NUMBER_OF_BUTTONS] = {BTN_0_PIN, BTN_1_PIN, BTN_2_PIN, BTN_3_PIN};

int timer_hold[NUMBER_OF_BUTTONS];
int current_state[NUMBER_OF_BUTTONS];
volatile int button_flag[NUMBER_OF_BUTTONS];
volatile int hold_flag[NUMBER_OF_BUTTONS];

void btnInit(){
	for (int i = 0; i < 0; i++){
		input0[i] = NORMAL_STATE;
		input1[i] = NORMAL_STATE;
		input2[i] = NORMAL_STATE;
		input3[i] = NORMAL_STATE;

		timerHold[i] = TIMER_HOLD_CHECK;
		currentState[i] = 0;
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
