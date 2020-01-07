#include "simpleRoundRobin.h"
#include "stm32f4xx.h"

/* This is an implementation of an extremely simple (and inefficient!) round-robin scheduler.

   An array of pointers to TCBs is declared, and when tasks are added they are inserted into
	 this array.  When tasks finish, they are removed from the array (pointers are set back to
	 zero).  When the scheduler is invoked, it simply looks for the next available non-zero
	 pointer in the array, and returns it.  If there are no non-zero TCB pointers in the array,
	 a pointer to the idle task is returned instead.
	 
	 The inefficiency in this implementation arises mainly from the way arrays are used for list
	 storage.  If the array is large and the number of tasks is small, the scheduler will spend
	 a lot of time looking through the array each time it is called. */

/* Prototypes (functions are static, so these aren't in the header file) */
static OS_TCB_t const * simpleRoundRobin_scheduler(void);
static void simpleRoundRobin_wait(void * const reason, uint32_t checkCode);
static void simpleRoundRobin_notify(void * const reason);
static void simpleRoundRobin_addTask(OS_TCB_t * const tcb);
static void simpleRoundRobin_taskExit(OS_TCB_t * const tcb);

static OS_TCB_t * tasks[SIMPLE_RR_MAX_TASKS] = {0};

/* Scheduler block for the simple round-robin */
OS_Scheduler_t const simpleRoundRobinScheduler = {
	.preemptive = 1,
	.scheduler_callback = simpleRoundRobin_scheduler,
	.wait_callback = simpleRoundRobin_wait,
	.notify_callback = simpleRoundRobin_notify,
	.addtask_callback = simpleRoundRobin_addTask,
	.taskexit_callback = simpleRoundRobin_taskExit
};


/* Round-robin scheduler callback */
static OS_TCB_t const * simpleRoundRobin_scheduler(void) {
	static int i = 0;
	// Clear the yield flag if it's set - we simply don't care
	OS_currentTCB()->state &= ~TASK_STATE_YIELD;
	for (int j = 1; j <= SIMPLE_RR_MAX_TASKS; j++) {
		i = (i + 1) % SIMPLE_RR_MAX_TASKS;
		if (tasks[i] != 0 && !(tasks[i]->state & TASK_STATE_WAIT)) {
			if (!(tasks[i]->state & TASK_STATE_SLEEP)) {
				return tasks[i];
			}			
			// TODO: deal with overflow of time
			uint32_t const elapsedTicks = OS_elapsedTicks();
			if (elapsedTicks > tasks[i]->data) {
				// Do we need to clear the data field?
				tasks[i]->state &= ~TASK_STATE_SLEEP;
				return tasks[i];
			}					
		}
	}
	// No tasks in the list, so return the idle task
	return OS_idleTCB_p;
}

static void simpleRoundRobin_wait(void * const reason, uint32_t checkCode) {
	if (checkCode != OS_getCheckCode()) {
		return;
	}
	OS_currentTCB()->data = *(uint32_t *)reason;
	OS_currentTCB()->state |= TASK_STATE_WAIT;
	SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

static void simpleRoundRobin_notify(void * const reason) {
	for (int i = 0; i < SIMPLE_RR_MAX_TASKS; i++) {
		if (tasks[i] != 0) {
			if ((tasks[i]->state & TASK_STATE_WAIT) && (tasks[i]->data == *(uint32_t *)reason)) {
				tasks[i]->state &= ~TASK_STATE_WAIT;
				tasks[i]->data = 0;
			}
		}
	}
}

/* 'Add task' callback */
static void simpleRoundRobin_addTask(OS_TCB_t * const tcb) {
	for (int i = 0; i < SIMPLE_RR_MAX_TASKS; i++) {
		if (tasks[i] == 0) {
			tasks[i] = tcb;
			return;
		}
	}
	// If we get here, there are no free TCB slots, so the task just won't be added
}

/* 'Task exit' callback */
static void simpleRoundRobin_taskExit(OS_TCB_t * const tcb) {
	// Remove the given TCB from the list of tasks so it won't be run again
	for (int i = 0; i < SIMPLE_RR_MAX_TASKS; i++) {
		if (tasks[i] == tcb) {
			tasks[i] = 0;
		}
	}	
}
