#ifndef FSM_AUTOMATIC_H
#define FSM_AUTOMATIC_H

/* BUG-08 FIX: include global.h so callers get fsm_mode_t, fsm_state_t etc. */
#include "global.h"

/* Run the automatic FSM — called each iteration of the main while(1) loop */
void fsm_automatic_run(void);

#endif /* FSM_AUTOMATIC_H */
