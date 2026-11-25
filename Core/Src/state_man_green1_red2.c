#include "state_man_green1_red2.h"
#include "traffic_light.h"
#include "button.h"
#include "data_bus.h"
void state_man_green1_red2_enter(void){
    fsm_status = FSM_STATE_MAN_GREEN1_RED2;
    traffic_set(LANE_1, GREEN);
    traffic_set(LANE_2, RED);
    snprintf(first_row_buffer, sizeof(first_row_buffer), "MAN: G1-R2");
}
void state_man_green1_red2_run(void){
    if (button_flag[1]) {
        button_flag[1] = 0;
        extern void state_man_yellow1_red2_enter(void);
        state_man_yellow1_red2_enter();
    }
}