/*
 * global.h - Shared types and variables for traffic light FSM project
 */
#ifndef INC_GLOBAL_H_
#define INC_GLOBAL_H_

#include "main.h"
#include <stdint.h>

#include "software_timer.h"

/* Compatibility with traffic light module naming */
#define LANE_1 1
#define LANE_2 2

/* Traffic light color codes follow traffic_light.h (OFF=0, YELLOW=1, GREEN=2, RED=3) */
#define LIGHT_OFF    0
#define LIGHT_YELLOW 1
#define LIGHT_GREEN  2
#define LIGHT_RED    3

/* Operation modes */
typedef enum {
    FSM_MODE_AUTOMATIC = 0,
    FSM_MODE_MANUAL,
    FSM_MODE_TUNING
} fsm_mode_t;

/* FSM states (all modes) */
typedef enum {
    FSM_STATE_INIT = 0,

    /* Automatic mode states */
    FSM_STATE_AUTO_RED1_GREEN2,
    FSM_STATE_AUTO_RED1_YELLOW2,
    FSM_STATE_AUTO_GREEN1_RED2,
    FSM_STATE_AUTO_YELLOW1_RED2,

    /* Manual mode states */
    FSM_STATE_MAN_ALL_RED,
    FSM_STATE_MAN_RED1_GREEN2,
    FSM_STATE_MAN_RED1_YELLOW2,
    FSM_STATE_MAN_GREEN1_RED2,
    FSM_STATE_MAN_YELLOW1_RED2,

    /* Tuning mode states */
    FSM_STATE_TUNE_RED,
    FSM_STATE_TUNE_GREEN,
    FSM_STATE_TUNE_YELLOW,
    FSM_STATE_TUNE_INVALID
} fsm_state_t;

/* Shared FSM state/mode */
extern fsm_state_t fsm_status;
extern fsm_mode_t  fsm_mode;

/* Light durations in milliseconds (rule: time_red == time_green + time_yellow) */
extern int time_red;
extern int time_green;
extern int time_yellow;

/* LCD top line buffer (FSM writes; display task would read) */
extern char lcd_top[17];

/* DHT20 placeholders (skipped for now, kept for compatibility) */
extern int16_t g_temp_x10;
extern uint8_t g_humi;
extern uint8_t g_sensor_ok;

/* Button flags exposed by button module (set on press events) */
extern int button_flag[];

/* Timer counter exposed for manual disarm in mode transitions */
extern int timer_counter[];

void clearTimer(int timer_index);

#endif /* INC_GLOBAL_H_ */
