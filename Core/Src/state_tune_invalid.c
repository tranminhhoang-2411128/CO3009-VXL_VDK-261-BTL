#include "state_tune_invalid.h"
#include "fsm_tuning.h"
void state_tune_invalid_enter(void){
    fsm_status = FSM_STATE_TUNE_INVALID;
    fsm_tuning_on_state_enter(FSM_STATE_TUNE_INVALID);
}
void state_tune_invalid_run(void){
    fsm_tuning_on_state_run(FSM_STATE_TUNE_INVALID);
}
