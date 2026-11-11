#include "state_tune_yellow.h"
#include "fsm_tuning.h"
void state_tune_yellow_enter(void){
    fsm_status = FSM_STATE_TUNE_YELLOW;
    fsm_tuning_on_state_enter(FSM_STATE_TUNE_YELLOW);
}
void state_tune_yellow_run(void){
    fsm_tuning_on_state_run(FSM_STATE_TUNE_YELLOW);
}
