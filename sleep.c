#include "sleep.h"
#include "os_internal.h"

void OS_sleep(uint32_t time) {
	uint32_t const elapsedTicks = OS_elapsedTicks();
	OS_TCB_t *tcb = OS_currentTCB();
	
	tcb->data = elapsedTicks + time;
	tcb->state |= TASK_STATE_SLEEP;
	
	OS_yield();
}
