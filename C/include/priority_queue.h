#pragma once

#include <stdlib.h>
#include "keys.h"
#include "iterator.h"

//
// Implementation of a priority queue (smaller values first)
//

// PriorityQueue* opaque type
typedef struct _PriorityQueue PriorityQueue;
typedef struct _PriorityQueueIterator PriorityQueueIterator;

typedef enum {
  PQOrder_ascending = 1,
  PQOrder_descending = -1
} PQOrder;


// Type of objects returned by PriorityQueue iterators

typedef struct {
  void* elem;
  double priority;
} PQElem;

// Alloc the priority queue. If key_info is given it is used to
// alloc a dictionary that improves the speed of get_priority and
// increase_priority methods. If NULL the queue revert to slower methods
// for these functions.
PriorityQueue* PriorityQueue_new(PQOrder, KeyInfo* key_info);

// Returns the key info used to initialize the queue
KeyInfo* PriorityQueue_key_info(PriorityQueue*);

// Dealloc the priority queue
void PriorityQueue_free(PriorityQueue*);

// Pushes a new element with the given priority into the queue
void PriorityQueue_push(PriorityQueue* pq, void* elem, double priority);

// return 1 if the queue is empty. 0 otherwise.
int PriorityQueue_empty(PriorityQueue*);

// Returns the number of elements in the queue.
size_t PriorityQueue_size(PriorityQueue*);

// Pops the top element from the queue.
void PriorityQueue_pop(PriorityQueue*);

// Returns the top value in the priority queue
void*  PriorityQueue_top_value(PriorityQueue*);

// Returns the priority of the top element
double PriorityQueue_top_priority(PriorityQueue*);

// Retrieves the priority of the given element. Note that this is a O(n) operation.
// It returns 1 and stores the result into "result" if elem is found in the priority queue
// It returns 0 otherwise.
// Note: the comparison between elements is performed using pointer equality.
int PriorityQueue_get_priority(PriorityQueue* pq, void* elem, double* result);

// Decreases the priority of the given element. The element *must* belong to the priority
// queue AND new_priority *must* be lower than the previous one.
// If elem does not belong to the priority queue or new_priority > old_priority,
// an error is raised.
void   PriorityQueue_decrease_priority(PriorityQueue* pq, void* elem, double new_priority);

// Try to decrease the priority of the given element. If either the element does not belong
// to the priority queue or if the new priority is not lower than the old one, nothing is done.
int PriorityQueue_try_decrease_priority(PriorityQueue* pq, void* elem, double priority);

// Creates a new PriorityQueueIterator
PriorityQueueIterator* PriorityQueueIterator_new(PriorityQueue*);

// Frees the priority queue iterator
void PriorityQueueIterator_free(PriorityQueueIterator*);

// Advances the iterator
void PriorityQueueIterator_next(PriorityQueueIterator*);

// Returns the priority queue element on which the iterator is pointing
PQElem* PriorityQueueIterator_get(PriorityQueueIterator*);

//  Returns 1 if the iterator is past its end
int PriorityQueueIterator_end(PriorityQueueIterator*);

// Returns 1 if the two iterators point at the same position of the same container
int PriorityQueueIterator_same(PriorityQueueIterator*, PriorityQueueIterator*);

// Iterator interface

Iterator PriorityQueue_it(PriorityQueue*);
