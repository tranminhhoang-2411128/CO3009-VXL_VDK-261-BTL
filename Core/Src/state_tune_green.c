#include "state_tune_green.h"
#include "fsm_tuning.h"
void state_tune_green_enter(void){
    fsm_status = FSM_STATE_TUNE_GREEN;
    fsm_tuning_on_state_enter(FSM_STATE_TUNE_GREEN);
}
void state_tune_green_run(void){
    fsm_tuning_on_state_run(FSM_STATE_TUNE_GREEN);
}
