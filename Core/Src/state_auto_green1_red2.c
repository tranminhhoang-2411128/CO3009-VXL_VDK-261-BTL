#include "state_auto_green1_red2.h"
#include "traffic_light.h"
#include "software_timer.h"
#define TIMER_TRAFFIC 0
void state_auto_green1_red2_enter(void){
    fsm_status = FSM_STATE_AUTO_GREEN1_RED2;
    traffic_set(LANE_1, GREEN);
    traffic_set(LANE_2, RED);
    setTimer(TIMER_TRAFFIC, time_green);
}
void state_auto_green1_red2_run(void){
    if (timer_flag[TIMER_TRAFFIC]) {
        timer_flag[TIMER_TRAFFIC] = 0;
        extern void state_auto_yellow1_red2_enter(void);
        state_auto_yellow1_red2_enter();
    }
}
