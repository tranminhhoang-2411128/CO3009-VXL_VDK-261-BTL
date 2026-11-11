#ifndef FSM_MANUAL_H
#define FSM_MANUAL_H

#include "global.h"

/* BUG-13 FIX: removed corrupted \r\n line that broke every TU including this header */

/* Manual mode FSM — called each iteration of the main while(1) loop */
void fsm_manual_run(void);

#endif /* FSM_MANUAL_H */
