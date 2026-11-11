#include "state_tune_red.h"
#include "fsm_tuning.h"
void state_tune_red_enter(void){
    fsm_status = FSM_STATE_TUNE_RED;
    fsm_tuning_on_state_enter(FSM_STATE_TUNE_RED);
}
void state_tune_red_run(void){
    fsm_tuning_on_state_run(FSM_STATE_TUNE_RED);
}
