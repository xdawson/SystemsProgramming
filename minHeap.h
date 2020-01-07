#ifndef MINHEAP_H
#define MINHEAP_H

#include "os.h"

//OS_TCB_t ** heap_init(int maxSize);
//void heap_free(OS_TCB_t ** heap);






OS_TCB_t * heap_extractMin(OS_TCB_t ** heap);
void heap_insert(OS_TCB_t ** heap, OS_TCB_t * newNode);
//void heap_delete(OS_TCB_t * root, OS_TCB_t * newNode);


#endif /* MINHEAP_H */
