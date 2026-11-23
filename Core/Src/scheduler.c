/*
 * scheduler.c
 *
 * cost of each function (n = number of tasks):
 *   SCH_Update          o(1)  - only counts ticks, safe to call in the isr
 *   SCH_Delete_Task     o(1)  - list is double linked
 *   SCH_Dispatch_Tasks  o(1)  per tick + per task that is due
 *   SCH_Add_Task        o(n)  - needs to find its place in the sorted list
 *
 * the list is only changed in the main loop, the isr touches nothing
 * but the tick counter, so no need to disable interrupts.
 */

#include "scheduler.h"

#define NONE            0xFF

#define TASK_FREE       0
#define TASK_WAITING    1
#define TASK_RUNNING    2

static sTask SCH_tasks_G[SCH_MAX_TASKS];

static uint8_t head;        // first task in the delta list
static uint8_t free_head;   // first free slot

// written by isr, read by main loop
static volatile uint32_t tick_count;
// ticks already handled by the main loop
static uint32_t tick_done;

/* ------------------------------------------------------------------ */
/* private helpers                                                      */
/* ------------------------------------------------------------------ */

// ms to ticks, round up so a small delay is not lost
static uint32_t ms_to_ticks(uint32_t ms) {
	return (ms + SCH_TICK_MS - 1) / SCH_TICK_MS;
}

// put a slot back to the free list
static void free_slot(uint8_t id) {
	SCH_tasks_G[id].pTask = 0;
	SCH_tasks_G[id].state = TASK_FREE;
	SCH_tasks_G[id].prev = NONE;
	SCH_tasks_G[id].next = free_head;
	free_head = id;
}

// insert a task into the delta list, delay is counted from now
static void insert_task(uint8_t id, uint32_t delay) {
	uint8_t prev = NONE;
	uint8_t cur = head;

	// walk until the sum of deltas passes our delay
	// tasks with the same time keep the order they were added
	while (cur != NONE && SCH_tasks_G[cur].Delay <= delay) {
		delay -= SCH_tasks_G[cur].Delay;
		prev = cur;
		cur = SCH_tasks_G[cur].next;
	}

	SCH_tasks_G[id].Delay = delay;
	SCH_tasks_G[id].prev = prev;
	SCH_tasks_G[id].next = cur;
	SCH_tasks_G[id].state = TASK_WAITING;

	// the node after us now waits less
	if (cur != NONE) {
		SCH_tasks_G[cur].Delay -= delay;
		SCH_tasks_G[cur].prev = id;
	}

	if (prev != NONE) {
		SCH_tasks_G[prev].next = id;
	} else {
		head = id;
	}
}

// take a task out of the delta list
static void remove_task(uint8_t id) {
	uint8_t prev = SCH_tasks_G[id].prev;
	uint8_t next = SCH_tasks_G[id].next;

	// give our delay to the next node so its time does not change
	if (next != NONE) {
		SCH_tasks_G[next].Delay += SCH_tasks_G[id].Delay;
		SCH_tasks_G[next].prev = prev;
	}

	if (prev != NONE) {
		SCH_tasks_G[prev].next = next;
	} else {
		head = next;
	}
}

// run every task at the head whose delay reached 0
static void run_ready_tasks(void) {
	while (head != NONE && SCH_tasks_G[head].Delay == 0) {
		uint8_t id = head;

		remove_task(id);
		SCH_tasks_G[id].state = TASK_RUNNING;

		(*SCH_tasks_G[id].pTask)();

		// the task may have deleted itself while running
		if (SCH_tasks_G[id].state != TASK_RUNNING) {
			continue;
		}

		if (SCH_tasks_G[id].Period > 0) {
			insert_task(id, SCH_tasks_G[id].Period);
		} else {
			free_slot(id);
		}
	}
}

/* ------------------------------------------------------------------ */
/* public functions                                                     */
/* ------------------------------------------------------------------ */

void SCH_Init(void) {
	head = NONE;
	free_head = NONE;
	tick_count = 0;
	tick_done = 0;

	// build the free list from the last slot down so slot 0 is used first
	for (int i = SCH_MAX_TASKS - 1; i >= 0; i--) {
		SCH_tasks_G[i].TaskID = (uint8_t) i;
		SCH_tasks_G[i].Period = 0;
		SCH_tasks_G[i].Delay = 0;
		free_slot((uint8_t) i);
	}
}

// call this in HAL_TIM_PeriodElapsedCallback, once per tick
void SCH_Update(void) {
	tick_count++;
}

// call this in the main while(1)
void SCH_Dispatch_Tasks(void) {
	// tasks added with delay 0 run right away
	run_ready_tasks();

	// handle every tick that came since the last call
	// unsigned subtract still works when tick_count wraps around
	while ((uint32_t) (tick_count - tick_done) > 0) {
		tick_done++;

		// after run_ready_tasks the head always has delay > 0
		if (head != NONE && SCH_tasks_G[head].Delay > 0) {
			SCH_tasks_G[head].Delay--;
		}

		run_ready_tasks();
	}
}

// delay and period are in ms, period = 0 means one shot
// returns the task id, or NO_TASK_ID if the array is full
// do not call this from an isr
uint8_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD) {
	if (pFunction == 0 || free_head == NONE) {
		return NO_TASK_ID;
	}

	// take the first free slot
	uint8_t id = free_head;
	free_head = SCH_tasks_G[id].next;

	SCH_tasks_G[id].pTask = pFunction;
	SCH_tasks_G[id].Period = ms_to_ticks(PERIOD);

	insert_task(id, ms_to_ticks(DELAY));

	return id;
}

// returns 1 if the task was deleted, 0 if the id is not valid
// do not call this from an isr
uint8_t SCH_Delete_Task(uint8_t TaskID) {
	if (TaskID >= SCH_MAX_TASKS) {
		return 0;
	}

	switch (SCH_tasks_G[TaskID].state) {
	case TASK_WAITING:
		remove_task(TaskID);
		free_slot(TaskID);
		return 1;
	case TASK_RUNNING:
		// it is already out of the list, just stop it from being added back
		free_slot(TaskID);
		return 1;
	default:
		break;
	}

	return 0;
}
