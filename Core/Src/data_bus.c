/*
 * data_bus.c
 *
 *  Created on: Nov 23, 2026
 *      Author: Hoang
 */

#include "data_bus.h"

int data_events = 0;
char first_row_buffer[LCD_COLS + 1] = "row_0_default";
char second_row_buffer[LCD_COLS + 1]  = "row_1_default";

void data_publish(int evt){
	data_events =  data_events | evt;
}

void buffer_update()
{
    int evt = data_events;
    if (evt == 0) return;
    data_events = 0;

    if (evt & EVT_DHT20_NEW) {
		snprintf(second_row_buffer, sizeof(second_row_buffer),
		         "T:%4.1f%cC H:%4.1f%%", temp, 223, humi);
    }
    if (evt & EVT_TRAFFIC_NEW) {
        /* format traffic_light_buffer here */
    }
}
