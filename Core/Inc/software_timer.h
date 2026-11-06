/*
 * software_timer.h
 *
 *  Created on: Oct 26, 2026
 *      Author: Hoang
 */

#ifndef INC_SOFTWARE_TIMER_H_
#define INC_SOFTWARE_TIMER_H_

#define ISR_TICK 10
#define NUMBER_OF_S_TIMERS 3

extern volatile int timer_flag[NUMBER_OF_S_TIMERS];

void timerInit();
void setTimer(int timer_index, int timer_duration);
void timerRun();

#endif /* INC_SOFTWARE_TIMER_H_ */
