#include "priority_queue.h"
#include <stdlib.h>
#include <stdio.h>


#define PRIORITY_QUEUE_INITIAL_CAPACITY 1024

typedef struct {
  void* elem;
  double priority;
} PQElem;

struct _PriorityQueue {
  PQElem* array;
  unsigned int size;
  unsigned int capacity;
};

static void PQElem_swap(PQElem* e1, PQElem* e2) {
  PQElem tmp = *e1;
  *e1 = *e2;
  *e2 = tmp;
}

static unsigned int PriorityQueue_parent(unsigned int pos) {
  return (pos - 1) / 2;
}

static unsigned int PriorityQueue_left_child(unsigned int pos) {
  return pos * 2 + 1;
}

static unsigned int PriorityQueue_right_child(unsigned int pos) {
  return pos * 2 + 2;
}

PriorityQueue PriorityQueue_new() {
  PriorityQueue result = (PriorityQueue) malloc(sizeof(struct _PriorityQueue));
  result->array = (PQElem*) malloc(sizeof(PQElem) * PRIORITY_QUEUE_INITIAL_CAPACITY);
  result->size = 0;
  result->capacity = PRIORITY_QUEUE_INITIAL_CAPACITY;

  return result;
}

void PriorityQueue_free(PriorityQueue pq) {
  free(pq->array);
  free(pq);
}

unsigned int PriorityQueue_size(PriorityQueue pq) {
  return pq->size;
}

static void PriorityQueue_try_realloc(PriorityQueue pq) {
  if(pq->capacity > pq->size) {
    return;
  }
  pq->capacity *= 2;
  pq->array = (PQElem*) realloc(pq->array, sizeof(PQElem) * pq->capacity );
}

static void PriorityQueue_moveup(PriorityQueue pq, unsigned int pos) {
  unsigned int parent = PriorityQueue_parent(pos);
  while(pos != 0 && pq->array[parent].priority > pq->array[pos].priority) {
    PQElem_swap(&pq->array[parent], &pq->array[pos]);
    pos = parent;
    parent = PriorityQueue_parent(pos);
  }
}

static void PriorityQueue_movedown(PriorityQueue pq, unsigned int pos) {
  unsigned int left = PriorityQueue_left_child(pos);
  unsigned int right = PriorityQueue_right_child(pos);
  if(left >= pq->size && right >= pq->size) {
    return;
  }

  unsigned int best;
  if(left < pq->size && right < pq->size) {
    best = pq->array[left].priority < pq->array[right].priority ? left : right;
  } else if(left < pq->size) {
    best = left;
  } else {
    best = right;
  }

  if(pq->array[best].priority < pq->array[pos].priority) {
    PQElem_swap(&pq->array[best], &pq->array[pos]);
    PriorityQueue_movedown(pq, best);
  }
}

void PriorityQueue_push(PriorityQueue pq, void* elem, double priority) {
  PriorityQueue_try_realloc(pq);

  pq->array[pq->size].elem = elem;
  pq->array[pq->size].priority = priority;
  PriorityQueue_moveup(pq, pq->size);
  pq->size += 1;
}

int PriorityQueue_empty(PriorityQueue pq)  {
  return pq->size == 0;
}

void*  PriorityQueue_top_value(PriorityQueue pq) {
  return pq->array[0].elem;
}

double PriorityQueue_top_priority(PriorityQueue pq) {
  return pq->array[0].priority;
}

void PriorityQueue_pop(PriorityQueue pq) {
  pq->array[0] = pq->array[pq->size-1];
  pq->size -= 1;
  PriorityQueue_movedown(pq, 0);
}


void PriorityQueue_decrease_priority(PriorityQueue pq, void* elem, double priority) {
  unsigned int i;
  for(i=0; i<pq->size; ++i) {
    if(pq->array[i].elem == elem) {
      break;
    }
  }

  if(i==pq->size) {
    printf("Could not find elem in the queue\n");
    exit(1);
  }

  pq->array[i].priority = priority;
  PriorityQueue_moveup(pq, i);
}
