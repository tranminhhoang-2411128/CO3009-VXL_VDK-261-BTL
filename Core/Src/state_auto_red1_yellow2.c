#include "state_auto_red1_yellow2.h"
#include "traffic_light.h"
#include "software_timer.h"
#define TIMER_TRAFFIC 0
void state_auto_red1_yellow2_enter(void){
    fsm_status = FSM_STATE_AUTO_RED1_YELLOW2;
    traffic_set(LANE_1, RED);
    traffic_set(LANE_2, YELLOW);
    setTimer(TIMER_TRAFFIC, time_yellow);
}
void state_auto_red1_yellow2_run(void){
    if (timer_flag[TIMER_TRAFFIC]) {
        timer_flag[TIMER_TRAFFIC] = 0;
        extern void state_auto_green1_red2_enter(void);
        state_auto_green1_red2_enter();
    }
}
