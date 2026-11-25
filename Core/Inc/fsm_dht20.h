/*
 * fsm_dht20.h
 *
 *  Created on: Nov 23, 2026
 *      Author: Hoang
 */

#ifndef INC_FSM_DHT20_H_
#define INC_FSM_DHT20_H_

#include "dht20.h"
#include "data_bus.h"

void fsm_dht20_init();
void fsm_dht20_run();
/* DHT20_flag
 * 0: not connected
 * 1: connected, no measurement triggered
 * 2: measurement triggered, waiting for busy bit
 * 3: measurement completed
 */

#endif /* INC_FSM_DHT20_H_ */
