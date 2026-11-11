/*
 * traffic_light.c
 *
 *  Created on: Nov 7, 2026
 *      Author: Hoang
 */

#include "traffic_light.h"

void trafficSet(int lane, int state){
	switch (lane) {
		case 1:
	    	HAL_GPIO_WritePin(TL1_A_GPIO_Port, TL1_A_Pin, (state >> 1) & 1);
		    HAL_GPIO_WritePin(TL1_B_GPIO_Port, TL1_B_Pin, state & 1);
			break;

		case 2:
	    	HAL_GPIO_WritePin(TL2_A_GPIO_Port, TL2_A_Pin, (state >> 1) & 1);
		    HAL_GPIO_WritePin(TL2_B_GPIO_Port, TL2_B_Pin, state & 1);
			break;

		default:
			break;
	}
}

/* Compatibility wrapper for FSM code using traffic_set(lane, color) */
void traffic_set(int lane, int color)
{
	trafficSet(lane, color);
}

