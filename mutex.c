#include "mutex.h"
#include "stm32f4xx.h"
#include "os.h"

void OS_mutex_init(OS_mutex_t *mutex) {
	// TODO: Add guard for nullptr arg
	mutex->tcb = 0;
	mutex->count = 0;
}

void OS_mutex_acquire(OS_mutex_t *mutex) {
	// TODO: Add guard for nullptr arg
	while(1) {
		OS_TCB_t *tcb = (OS_TCB_t *)__LDREXW((uint32_t *)&mutex->tcb);
		
		if (!tcb) {
			uint32_t retCode = __STREXW((uint32_t)OS_currentTCB(), (uint32_t *)&mutex->tcb);			
			// Failed store
			if (retCode == 1)
				continue;
			
			break;			
		}
		else if (tcb != OS_currentTCB()) {
			OS_wait((void *)mutex, OS_getCheckCode());
			continue;
		}
		// The task control blocks are the same, this is a recursive mutex so
		// we are allowing it to ack the mutex again
		else {
			break;
		}
	}
	
	mutex->count++;
}

void OS_mutex_release(OS_mutex_t *mutex) {
	// TODO: Add guard for nullptr arg
	// Check the mutex is owned by the task calling it
	if (OS_currentTCB() != mutex->tcb)
		return;
	
	mutex->count--;
	
	if (mutex->count == 0) {
		mutex->tcb = 0;
		OS_notify((void *)mutex);
	}
}
