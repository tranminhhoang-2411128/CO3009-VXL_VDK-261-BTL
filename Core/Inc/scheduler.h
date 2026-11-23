/*
 * scheduler.h
 *
 * cooperative scheduler with o(1) update.
 * tasks are kept in a list sorted by time, each node stores its delay
 * relative to the node before it (delta list), so the timer isr only
 * needs to count ticks and never walks the list.
 */

 #ifndef INC_SCHEDULER_H_
 #define INC_SCHEDULER_H_
 
 // no hal needed here, so the module also builds on pc for testing
 #include <stdint.h>
 
 // must match the timer interrupt period (ms)
 #define SCH_TICK_MS     10
 
 // max number of tasks at the same time
 #define SCH_MAX_TASKS   10
 
 // returned when a task can not be added
 #define NO_TASK_ID      0xFF
 
 typedef struct {
	 void (*pTask)(void);    // task function
	 uint32_t Delay;         // ticks to wait after the previous node in the list
	 uint32_t Period;        // period in ticks, 0 means run once
	 uint8_t TaskID;         // index of this task in the array
	 uint8_t next;           // next node in the list (or free list)
	 uint8_t prev;           // previous node in the list
	 uint8_t state;          // free / waiting / running
 } sTask;
 
 void SCH_Init(void);
 void SCH_Update(void);
 void SCH_Dispatch_Tasks(void);
 uint8_t SCH_Add_Task(void (*pFunction)(void), uint32_t DELAY, uint32_t PERIOD);
 uint8_t SCH_Delete_Task(uint8_t TaskID);
 
 #endif /* INC_SCHEDULER_H_ */