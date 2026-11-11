#include "state_man_all_red.h"
#include "traffic_light.h"
#include "button.h"
#include <stdio.h>
void state_man_all_red_enter(void){
    fsm_status = FSM_STATE_MAN_ALL_RED;
    traffic_set(LANE_1, RED);
    traffic_set(LANE_2, RED);
    //snprintf(lcd_top, sizeof(lcd_top), "MODE: MANUAL");
}
void state_man_all_red_run(void){
    if (button_flag[1]) {
        button_flag[1] = 0;
        extern void state_man_red1_green2_enter(void);
        state_man_red1_green2_enter();
    }
}
