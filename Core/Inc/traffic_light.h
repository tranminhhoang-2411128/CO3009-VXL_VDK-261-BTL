/*
 * traffic_light.h
 *
 *  Created on: Nov 7, 2026
 *      Author: Hoang
 */

#ifndef INC_TRAFFIC_LIGHT_H_
#define INC_TRAFFIC_LIGHT_H_

#include "global.h"

#define TL1_A_GPIO_Port D4_GPIO_Port
#define TL1_A_Pin D4_Pin
#define TL1_B_GPIO_Port D3_GPIO_Port
#define TL1_B_Pin D3_Pin
#define TL2_A_GPIO_Port D6_GPIO_Port
#define TL2_A_Pin D6_Pin
#define TL2_B_GPIO_Port D5_GPIO_Port
#define TL2_B_Pin D5_Pin

//Based on the hardware traffic light module
#define OFF 0
#define YELLOW 2
#define GREEN 1
#define RED 3

void trafficSet(int lane, int state);
void traffic_set(int lane, int color);

#endif /* INC_TRAFFIC_LIGHT_H_ */

