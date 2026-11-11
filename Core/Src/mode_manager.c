#include "mode_manager.h"
#include "global.h"
#include "button.h"
#include "traffic_light.h"
#include "software_timer.h"
#include "fsm_tuning.h"
#include "state_man_all_red.h"

void mode_manager_run(void)
{
    /* In TUNING mode, BTN0 confirm/exit is handled by tuning FSM */
    if (fsm_mode == FSM_MODE_TUNING) return;

    if (!button_flag[0]) return; /* BTN0 not pressed */
    button_flag[0] = 0;          /* consume */

    switch (fsm_mode) {
    case FSM_MODE_AUTOMATIC:
        /* AUTO -> MANUAL: stop auto timer, enter safe MAN_ALL_RED */
        clearTimer(0);
        traffic_set(LANE_1, RED);
        traffic_set(LANE_2, RED);
        fsm_mode = FSM_MODE_MANUAL;
        state_man_all_red_enter();
        break;

    case FSM_MODE_MANUAL:
        /* MANUAL -> TUNING: keep both red (should already be safe at entry transitions),
           initialize tuning temps/state */
        traffic_set(LANE_1, RED);
        traffic_set(LANE_2, RED);
        fsm_mode = FSM_MODE_TUNING;
        fsm_tuning_init();
        break;

    default:
        break;
    }
}
