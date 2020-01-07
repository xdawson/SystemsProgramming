#ifndef MUTEX_H
#define MUTEX_H

#include "task.h"
#include <stdint.h>

typedef struct {
	OS_TCB_t *tcb;
	uint32_t count;
} OS_mutex_t;

void OS_mutex_init(OS_mutex_t *mutex);
void OS_mutex_acquire(OS_mutex_t *mutex);
void OS_mutex_release(OS_mutex_t *mutex);

#endif /* MUTEX_H */
