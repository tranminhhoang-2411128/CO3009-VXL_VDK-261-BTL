#include "fsm_automatic.h"
#include "global.h"
#include "data_bus.h"
#include "state_auto_red1_green2.h"
#include "state_auto_red1_yellow2.h"
#include "state_auto_green1_red2.h"
#include "state_auto_yellow1_red2.h"
#include <stdio.h>

void fsm_automatic_run(void)
{
    if (fsm_mode != FSM_MODE_AUTOMATIC) return;

    /* AUTO_INIT: run once on reset or after tuning confirm */
    if (fsm_status == FSM_STATE_INIT) {
        snprintf(first_row_buffer, sizeof(first_row_buffer), "MODE: AUTO");
        state_auto_red1_green2_enter();
        return;
    }

    switch (fsm_status) {
    case FSM_STATE_AUTO_RED1_GREEN2:
        state_auto_red1_green2_run();
        break;
    case FSM_STATE_AUTO_RED1_YELLOW2:
        state_auto_red1_yellow2_run();
        break;
    case FSM_STATE_AUTO_GREEN1_RED2:
        state_auto_green1_red2_run();
        break;
    case FSM_STATE_AUTO_YELLOW1_RED2:
        state_auto_yellow1_red2_run();
        break;
    default:
        /* Recover to safe init */
        fsm_status = FSM_STATE_INIT;
        break;
    }
}
