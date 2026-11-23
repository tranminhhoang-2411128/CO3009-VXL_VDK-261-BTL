/*
 * data_bus.h
 *
 *  Created on: Nov 23, 2026
 *      Author: Hoang
 */

#ifndef INC_DATA_BUS_H_
#define INC_DATA_BUS_H_

#include "global.h"
#include "dht20.h"
#include "lcd.h"

extern int data_events;

void data_publish(int evt);
void buffer_update();

#endif /* INC_DATA_BUS_H_ */
