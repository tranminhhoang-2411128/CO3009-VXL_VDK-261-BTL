#include "fsm_tuning.h"
#include "global.h"
#include "data_bus.h"
#include "button.h"
#include "traffic_light.h"
#include "software_timer.h"
#include "state_tune_red.h"
#include "state_tune_green.h"
#include "state_tune_yellow.h"
#include "state_tune_invalid.h"
#include "state_auto_red1_green2.h"
#include <stdio.h>

#define TIMER_TUNING_ERROR 2

static int temp_red;
static int temp_green;
static int temp_yellow;

static void update_tuning_display(void)
{
    switch (fsm_status) {
    case FSM_STATE_TUNE_RED:
        snprintf(first_row_buffer, sizeof(first_row_buffer), "TUNE RED:%2ds", temp_red);
        break;
    case FSM_STATE_TUNE_GREEN:
        snprintf(first_row_buffer, sizeof(first_row_buffer), "TUNE GREEN:%2ds", temp_green);
        break;
    case FSM_STATE_TUNE_YELLOW:
        snprintf(first_row_buffer, sizeof(first_row_buffer), "TUNE YELLOW:%2ds", temp_yellow);
        break;
    case FSM_STATE_TUNE_INVALID:
        snprintf(first_row_buffer, sizeof(first_row_buffer), "INVALID");
        break;
    default:
        break;
    }
}

void fsm_tuning_init(void)
{
    /* Load temps from current configured times (seconds) */
    temp_red = time_red / 1000;
    temp_green = time_green / 1000;
    temp_yellow = time_yellow / 1000;

    /* Safe state while tuning */
    traffic_set(LANE_1, RED);
    traffic_set(LANE_2, RED);

    clearTimer(TIMER_TUNING_ERROR);
    state_tune_red_enter();
}

void fsm_tuning_on_state_enter(fsm_state_t state)
{
    if (state == FSM_STATE_TUNE_INVALID) {
        snprintf(first_row_buffer, sizeof(first_row_buffer), "INVALID");
        setTimer(TIMER_TUNING_ERROR, 1500);
    } else {
        update_tuning_display();
    }
}

static int is_valid_tuning(void)
{
    if (temp_red < 1 || temp_red > 99) return 0;
    if (temp_green < 1 || temp_green > 99) return 0;
    if (temp_yellow < 1 || temp_yellow > 99) return 0;
    if (temp_red != temp_green + temp_yellow) return 0;
    if ((temp_red - temp_yellow) < 1) return 0;
    return 1;
}

void fsm_tuning_on_state_run(fsm_state_t state)
{
    /* Handle invalid display timeout */
    if (state == FSM_STATE_TUNE_INVALID) {
        if (timer_flag[TIMER_TUNING_ERROR]) {
            timer_flag[TIMER_TUNING_ERROR] = 0;
            /* Return to editing screen (default to RED) and keep old values in temps already reverted */
            state_tune_red_enter();
        }
        return;
    }

    /* BTN1: select next target RED -> GREEN -> YELLOW -> RED */
    if (button_flag[1]) {
        button_flag[1] = 0;
        if (state == FSM_STATE_TUNE_RED) state_tune_green_enter();
        else if (state == FSM_STATE_TUNE_GREEN) state_tune_yellow_enter();
        else state_tune_red_enter();
        return;
    }

    /* BTN2: increase 1s with constraint adjustments */
    if (button_flag[2]) {
        button_flag[2] = 0;
        if (state == FSM_STATE_TUNE_RED) {
            temp_red++;
            temp_green = temp_red - temp_yellow;
        } else if (state == FSM_STATE_TUNE_GREEN) {
            temp_green++;
            temp_red = temp_green + temp_yellow;
        } else if (state == FSM_STATE_TUNE_YELLOW) {
            temp_yellow++;
            temp_red = temp_green + temp_yellow;
        }
        update_tuning_display();
    }

    /* BTN3: decrease 1s with constraint adjustments */
    if (button_flag[3]) {
        button_flag[3] = 0;
        if (state == FSM_STATE_TUNE_RED) {
            temp_red--;
            temp_green = temp_red - temp_yellow;
        } else if (state == FSM_STATE_TUNE_GREEN) {
            temp_green--;
            temp_red = temp_green + temp_yellow;
        } else if (state == FSM_STATE_TUNE_YELLOW) {
            temp_yellow--;
            temp_red = temp_green + temp_yellow;
        }
        update_tuning_display();
    }

    /* BTN0: confirm and exit */
    if (button_flag[0]) {
        button_flag[0] = 0;
        if (is_valid_tuning()) {
            time_red = temp_red * 1000;
            time_green = temp_green * 1000;
            time_yellow = temp_yellow * 1000;
            /* Exit to AUTO via AUTO_INIT */
            fsm_mode = FSM_MODE_AUTOMATIC;
            fsm_status = FSM_STATE_INIT;
            snprintf(first_row_buffer, sizeof(first_row_buffer), "MODE: AUTO");
        } else {
            /* Revert temps to current valid globals and show INVALID */
            temp_red = time_red / 1000;
            temp_green = time_green / 1000;
            temp_yellow = time_yellow / 1000;
            state_tune_invalid_enter();
        }
    }
}

void fsm_tuning_run(void)
{
    if (fsm_mode != FSM_MODE_TUNING) return;

    switch (fsm_status) {
    case FSM_STATE_TUNE_RED:
        state_tune_red_run();
        break;
    case FSM_STATE_TUNE_GREEN:
        state_tune_green_run();
        break;
    case FSM_STATE_TUNE_YELLOW:
        state_tune_yellow_run();
        break;
    case FSM_STATE_TUNE_INVALID:
        state_tune_invalid_run();
        break;
    default:
        state_tune_red_enter();
        break;
    }
}
