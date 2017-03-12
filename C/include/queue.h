#pragma once

#include "list.h"

typedef List Queue;


// Creates a new queue and returns it
Queue* Queue_new(void);
// Frees all elements of the queue. All user objects are not
// dealloced
void Queue_free(Queue*);

// Enqueues a new object at the back of the queue
void Queue_enqueue(Queue*,void*);

// Dequeues an object from the front of the queue
void* Queue_dequeue(Queue*);

// Returns the front object (the next one that will be dequeued)
void* Queue_front(Queue*);

// Returns the queue size
size_t Queue_size(Queue*);

// Returns 1 iff the queue is empty
int Queue_empty(Queue*);
