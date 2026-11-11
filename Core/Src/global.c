#include "global.h"

fsm_state_t fsm_status = FSM_STATE_INIT;
fsm_mode_t  fsm_mode   = FSM_MODE_AUTOMATIC;

/* Default durations in ms: Red 10s, Green 7s, Yellow 3s */
int time_red    = 10000;
int time_green  =  7000;
int time_yellow =  3000;

char lcd_top[17] = "MODE: AUTO";

int16_t g_temp_x10  = 0;
uint8_t g_humi      = 0;
uint8_t g_sensor_ok = 0;

void clearTimer(int timer_index)
{
    if (timer_index >= 0 && timer_index < NUMBER_OF_S_TIMERS) {
        timer_flag[timer_index] = 0;
        timer_counter[timer_index] = -1;
    }
}
