#include "fsm_manual.h"
#include "global.h"
#include "state_man_all_red.h"
#include "state_man_red1_green2.h"
#include "state_man_red1_yellow2.h"
#include "state_man_green1_red2.h"
#include "state_man_yellow1_red2.h"

void fsm_manual_run(void)
{
    if (fsm_mode != FSM_MODE_MANUAL) return;

    switch (fsm_status) {
    case FSM_STATE_MAN_ALL_RED:
        state_man_all_red_run();
        break;
    case FSM_STATE_MAN_RED1_GREEN2:
        state_man_red1_green2_run();
        break;
    case FSM_STATE_MAN_RED1_YELLOW2:
        state_man_red1_yellow2_run();
        break;
    case FSM_STATE_MAN_GREEN1_RED2:
        state_man_green1_red2_run();
        break;
    case FSM_STATE_MAN_YELLOW1_RED2:
        state_man_yellow1_red2_run();
        break;
    default:
        state_man_all_red_enter();
        break;
    }
}
