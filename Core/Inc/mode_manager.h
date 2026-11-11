#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include "global.h"

/* Handle BTN0 mode-cycling: AUTO → MANUAL → TUNING → AUTO.
 * Performs the safe entry transition for each new mode:
 *   AUTO   → MANUAL : stop auto timer, set both lanes Red, fsm_status = MAN_ALL_RED
 *   MANUAL → TUNING : keep both lanes Red, lcd_top = current tune label
 *   TUNING → AUTO   : validate & save times if valid, then AUTO_INIT (via fsm_status = INIT)
 * Called every iteration of the main while(1) loop, before the individual FSMs. */
void mode_manager_run(void);

#endif /* MODE_MANAGER_H */
