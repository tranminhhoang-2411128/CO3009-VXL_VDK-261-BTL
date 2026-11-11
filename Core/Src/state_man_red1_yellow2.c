#include "state_man_red1_yellow2.h"
#include "traffic_light.h"
#include "button.h"
#include <stdio.h>
void state_man_red1_yellow2_enter(void){
    fsm_status = FSM_STATE_MAN_RED1_YELLOW2;
    traffic_set(LANE_1, RED);
    traffic_set(LANE_2, YELLOW);
    //snprintf(lcd_top, sizeof(lcd_top), "MAN: R1-Y2");
}
void state_man_red1_yellow2_run(void){
    if (button_flag[1]) {
        button_flag[1] = 0;
        extern void state_man_green1_red2_enter(void);
        state_man_green1_red2_enter();
    }
}
