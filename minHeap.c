#include "minHeap.h"
#include "fixedPriorityScheduler.h"
#include <stdlib.h>

static int heapSize = 0;
//static int heapMaxSize = 0;

// Starting position 0
// Left child at (2 * k) + 1
// Right child at (2 * k) + 2
// Parent at (k-1) / 2
//OS_TCB_t ** heap_init(int maxSize) {
//	heapMaxSize = maxSize;
//	OS_TCB_t ** heap = (OS_TCB_t **)malloc(sizeof(OS_TCB_t *) * maxSize);
//	return heap;
//}

//void heap_free(OS_TCB_t ** heap) {
//	free(heap);
//}

void percolateDown(int pos, OS_TCB_t ** heap) {
	int childPos;
	
	for (; (2 * pos) < heapSize; pos = childPos) {
		childPos = (2 * pos) + 1;
		if (heap[childPos]->priority > heap[pos]->priority) {
			childPos++;
		}
		else {
			// swap
			OS_TCB_t * tmp = heap[pos];
			heap[pos] = heap[childPos];
			heap[childPos] = tmp;
			continue;
		}
		if (heap[childPos]->priority > heap[pos]->priority) {
			break;
		}
		else {
			// swap
			OS_TCB_t * tmp = heap[pos];
			heap[pos] = heap[childPos];
			heap[childPos] = tmp;
		}		
	}	
}

OS_TCB_t * heap_extractMin(OS_TCB_t ** heap) {
	OS_TCB_t * min = heap[0];
	heap[0] = heap[--heapSize];
	percolateDown(0, heap);	
	return min;
}

void heap_insert(OS_TCB_t ** heap, OS_TCB_t * newNode) {
	if (heapSize >= FIXED_PRIORITY_MAX_TASKS) {
		return;
	}
	
	int pos = heapSize++;
	for(; (pos > 0) && (newNode->priority < heap[(pos - 1) / 2]->priority); pos = (pos - 1) / 2) {
		heap[pos] = heap[(pos - 1) / 2];	
	}
	
	heap[pos] = newNode;
}

//void heap_delete(OS_TCB_t *root, OS_TCB_t * newNode) {}
