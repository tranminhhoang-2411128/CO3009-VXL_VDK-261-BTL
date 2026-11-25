#include "state_auto_red1_green2.h"
#include "traffic_light.h"
#include "data_bus.h"
#define TIMER_TRAFFIC 2
void state_auto_red1_green2_enter(void){
    fsm_status = FSM_STATE_AUTO_RED1_GREEN2;
    traffic_set(LANE_1, RED);
    traffic_set(LANE_2, GREEN);
    snprintf(first_row_buffer, sizeof(first_row_buffer), "MODE: AUTO");
    setTimer(TIMER_TRAFFIC, time_green);
}
void state_auto_red1_green2_run(void){
    if (timer_flag[TIMER_TRAFFIC]) {
        timer_flag[TIMER_TRAFFIC] = 0;
        extern void state_auto_red1_yellow2_enter(void);
        state_auto_red1_yellow2_enter();
    }
}