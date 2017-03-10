#include "priority_queue.h"
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "mem.h"


#define PRIORITY_QUEUE_INITIAL_CAPACITY 1024

typedef struct {
  void* elem;
  double priority;
} PQElem;

struct _PriorityQueue {
  PQElem* array;
  PQOrder order;
  size_t size;
  size_t capacity;
};

static void PQElem_swap(PQElem* e1, PQElem* e2) {
  PQElem tmp = *e1;
  *e1 = *e2;
  *e2 = tmp;
}

static size_t PriorityQueue_parent(size_t pos) {
  return (pos - 1) / 2;
}

static size_t PriorityQueue_left_child(size_t pos) {
  return pos * 2 + 1;
}

static size_t PriorityQueue_right_child(size_t pos) {
  return pos * 2 + 2;
}

PriorityQueue PriorityQueue_new(PQOrder order) {
  PriorityQueue result = (PriorityQueue) Mem_alloc(sizeof(struct _PriorityQueue));
  result->array = (PQElem*) Mem_alloc(sizeof(PQElem*) * PRIORITY_QUEUE_INITIAL_CAPACITY);
  result->size = 0;
  result->capacity = PRIORITY_QUEUE_INITIAL_CAPACITY;
  result->order = order;

  return result;
}

void PriorityQueue_free(PriorityQueue pq) {
  Mem_free(pq->array);
  Mem_free(pq);
}

size_t PriorityQueue_size(PriorityQueue pq) {
  return pq->size;
}

static int pq_key_cmp(PriorityQueue pq, double p1, double p2) {
  if(pq->order == PQOrder_ascending) {
    return p1 > p2;
  } else {
    return p1 < p2;
  }
}

static void PriorityQueue_try_realloc(PriorityQueue pq) {
  if(pq->capacity > pq->size) {
    return;
  }
  pq->capacity *= 2;
  pq->array = (PQElem*)Mem_realloc(pq->array, sizeof(PQElem*) * pq->capacity );
}

static void PriorityQueue_moveup(PriorityQueue pq, size_t pos) {
  size_t parent = PriorityQueue_parent(pos);
  while(pos != 0 && pq_key_cmp(pq, pq->array[parent].priority, pq->array[pos].priority)) {
    PQElem_swap(&pq->array[parent], &pq->array[pos]);
    pos = parent;
    parent = PriorityQueue_parent(pos);
  }
}

static void PriorityQueue_movedown(PriorityQueue pq, size_t pos) {
  size_t left = PriorityQueue_left_child(pos);
  size_t right = PriorityQueue_right_child(pos);
  if(left >= pq->size && right >= pq->size) {
    return;
  }

  size_t best;
  if(left < pq->size && right < pq->size) {
    best = !pq_key_cmp(pq, pq->array[left].priority, pq->array[right].priority) ? left : right;
  } else if(left < pq->size) {
    best = left;
  } else {
    best = right;
  }

  if(!pq_key_cmp(pq, pq->array[best].priority, pq->array[pos].priority)) {
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
  size_t i;
  for(i=0; i<pq->size; ++i) {
    if(pq->array[i].elem == elem) {
      break;
    }
  }

  if(i==pq->size) {
    Error_raise(Error_new(ERROR_GENERIC, "Priority queues: cannot find element while decreasing its priority"));
  }

  pq->array[i].priority = priority;
  PriorityQueue_moveup(pq, i);
}


void PriorityQueue_foreach(PriorityQueue pq, void (^callback)(void* elem, double priority)) {
  for(size_t i=0; i<pq->size; ++i) {
    callback(pq->array[i].elem, pq->array[i].priority);
  }
}
