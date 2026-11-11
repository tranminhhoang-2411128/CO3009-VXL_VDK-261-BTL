#ifndef FSM_TUNING_H
#define FSM_TUNING_H

#include "global.h"

/* Initialise tuning mode when entering from manual mode */
void fsm_tuning_init(void);
/* Run tuning FSM dispatch */
void fsm_tuning_run(void);

/* State module callbacks */
void fsm_tuning_on_state_enter(fsm_state_t state);
void fsm_tuning_on_state_run(fsm_state_t state);

#endif /* FSM_TUNING_H */
