#include "fixedPriorityScheduler.h"
#include "minHeap.h"

static OS_TCB_t const * fixedPriority_scheduler(void);
static void fixedPriority_addTask(OS_TCB_t * const tcb);

static OS_TCB_t * tasks[FIXED_PRIORITY_MAX_TASKS] = {0};

OS_Scheduler_t const fixedPriorityScheduler = {
	.preemptive = 1,
	.scheduler_callback = fixedPriority_scheduler,
	.addtask_callback = fixedPriority_addTask
};

static OS_TCB_t const * fixedPriority_scheduler(void) {
	return tasks[0];
}

static void fixedPriority_addTask(OS_TCB_t * const tcb) {
	heap_insert(tasks, tcb);
	int x = 4;
}
