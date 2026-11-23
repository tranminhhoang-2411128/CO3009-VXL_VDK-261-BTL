/*
 * fsm_dht20.c
 *
 *  Created on: Nov 23, 2026
 *      Author: Hoang
 */

#include "fsm_dht20.h"

void fsm_dht20_init(){
	setTimer(0, DHT20_INIT_TIME);
}

void fsm_dht20_run(){
	if (timer_flag[0] == 1){
		switch (DHT20_flag) {
			case 0:
				DHT20_Connect();
				setTimer(0, DHT20_CALIB_TIME);
				break;

			case 1:
				DHT20_TriggerMeasurement();
				setTimer(0, DHT20_READ_TIME);
				break;

			case 2:
				DHT20_ReadData();
				setTimer(0, DHT20_CALIB_TIME);
				break;

			case 3:
				DHT20_flag = 1;
				data_publish(EVT_DHT20_NEW);
				setTimer(0, DHT20_READ_PERIOD);
				break;

			default:
				break;
		}
	  }
}
