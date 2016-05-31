#pragma once

#include "list.h"

typedef List Queue;

Queue Queue_new(void);
void Queue_free(Queue);

void Queue_enqueue(Queue,void*);
void* Queue_dequeue(Queue);
void* Queue_front(Queue);

size_t Queue_size(Queue);
int Queue_empty(Queue);
