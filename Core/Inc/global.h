/*
 * global.h
 *
 *  Created on: Nov 6, 2026
 *      Author: Hoang
 */

#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
#include "stdio.h"
#include "software_timer.h"

#define EVT_DHT20_NEW    (1 << 0)
#define EVT_TRAFFIC_NEW  (1 << 1)

extern I2C_HandleTypeDef hi2c1;

#endif /* INC_GLOBAL_H_ */
