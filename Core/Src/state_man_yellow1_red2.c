#include "state_man_yellow1_red2.h"
#include "traffic_light.h"
#include "button.h"
#include <stdio.h>
void state_man_yellow1_red2_enter(void){
    fsm_status = FSM_STATE_MAN_YELLOW1_RED2;
    traffic_set(LANE_1, YELLOW);
    traffic_set(LANE_2, RED);
    //snprintf(lcd_top, sizeof(lcd_top), "MAN: Y1-R2");
}
void state_man_yellow1_red2_run(void){
    if (button_flag[1]) {
        button_flag[1] = 0;
        extern void state_man_red1_green2_enter(void);
        state_man_red1_green2_enter();
    }
}
