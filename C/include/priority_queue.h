#ifndef __PRIORITY_QUEUE_H_J2XM2N29__
#define __PRIORITY_QUEUE value_H_J2XM2N29__

typedef struct _PriorityQueue* PriorityQueue;

PriorityQueue PriorityQueue_new();
void PriorityQueue_free();
void PriorityQueue_push(PriorityQueue, const void*, double);
int PriorityQueue_empty(PriorityQueue);
unsigned int PriorityQueue_size(PriorityQueue);

void PriorityQueue_pop(PriorityQueue);
const void*  PriorityQueue_top_value(PriorityQueue);
double PriorityQueue_top_priority(PriorityQueue);
void   PriorityQueue_decrease_priority(PriorityQueue, const void*, double);


#endif
