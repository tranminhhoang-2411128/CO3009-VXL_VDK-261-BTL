#include "state_auto_yellow1_red2.h"
#include "traffic_light.h"
#include "data_bus.h"
#define TIMER_TRAFFIC 2
void state_auto_yellow1_red2_enter(void){
    fsm_status = FSM_STATE_AUTO_YELLOW1_RED2;
    traffic_set(LANE_1, YELLOW);
    traffic_set(LANE_2, RED);
    setTimer(TIMER_TRAFFIC, time_yellow);
}
void state_auto_yellow1_red2_run(void){
    if (timer_flag[TIMER_TRAFFIC]) {
        timer_flag[TIMER_TRAFFIC] = 0;
        extern void state_auto_red1_green2_enter(void);
        state_auto_red1_green2_enter();
    }
}