/*
 * dht20.h
 *
 *  Created on: Nov 9, 2026
 *      Author: Hoang
 */

#ifndef INC_DHT20_H_
#define INC_DHT20_H_

#include "global.h"

#define DHT20_ADDR 0x38
#define DHT20_INIT_TIME 200
#define DHT20_CALIB_TIME 20
#define DHT20_READ_TIME 90
#define DHT20_READ_PERIOD 1010

void DHT20_Connect();
void DHT20_TriggerMeasurement();
void DHT20_ReadData();

extern float temp;
extern float humi;
/* DHT20_flag
 * 0: not connected
 * 1: connected, no measurement triggered
 * 2: measurement triggered, waiting for busy bit
 * 3: measurement completed
 */
extern int DHT20_flag;

#endif /* INC_DHT20_H_ */
