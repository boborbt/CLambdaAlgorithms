#ifndef __PRIORITY_QUEUE_H_J2XM2N29__
#define __PRIORITY_QUEUE value_H_J2XM2N29__

//
// Implementation of a priority queue (smaller values first)
//

typedef struct _PriorityQueue* PriorityQueue;

// Alloc the priority queue
PriorityQueue PriorityQueue_new();

// Dealloc the priority queue
void PriorityQueue_free();

// Pushes a new element with the given priority into the queue
void PriorityQueue_push(PriorityQueue pq, const void* elem, double priority);

// return 1 if the queue is empty. 0 otherwise.
int PriorityQueue_empty(PriorityQueue);

// Returns the number of elements in the queue.
unsigned int PriorityQueue_size(PriorityQueue);

// Pops the top element from the queue.
void PriorityQueue_pop(PriorityQueue);

// Returns the top value in the priority queue
const void*  PriorityQueue_top_value(PriorityQueue);

// Returns the priority of the top element
double PriorityQueue_top_priority(PriorityQueue);

// Decreases the priority of the given element
void   PriorityQueue_decrease_priority(PriorityQueue pq, const void* elem, double new_priority);


#endif
