#include "priority_queue.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "errors.h"
#include "dictionary.h"
#include "mem.h"
#include "iterator_functions.h"

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
  Dictionary* index;
};

static void PQElem_swap(PriorityQueue* pq, size_t i, size_t j) {
  if(pq->index) {
    void* obj1;
    void* obj2;
    Dictionary_get(pq->index, pq->array[i].elem, &obj1);
    Dictionary_get(pq->index, pq->array[j].elem, &obj2);
    Dictionary_set(pq->index, pq->array[i].elem, obj2);
    Dictionary_set(pq->index, pq->array[j].elem, obj1);
  }

  PQElem tmp = pq->array[i];
  pq->array[i] = pq->array[j];
  pq->array[j] = tmp;
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

PriorityQueue* PriorityQueue_new(PQOrder order, KeyInfo* key_info) {
  PriorityQueue* result = (PriorityQueue*) Mem_alloc(sizeof(struct _PriorityQueue));
  result->array = (PQElem*) Mem_alloc(sizeof(PQElem) * PRIORITY_QUEUE_INITIAL_CAPACITY);
  result->size = 0;
  result->capacity = PRIORITY_QUEUE_INITIAL_CAPACITY;
  result->order = order;

  if(key_info) {
    result->index = Dictionary_new(key_info);
  } else {
    result->index = NULL;
  }

  return result;
}

void PriorityQueue_free(PriorityQueue* pq) {
  Mem_free(pq->array);
  Mem_free(pq);

  if(pq->index) {
    for_each(Dictionary_it(pq->index), ^(void* obj) {
      KeyValue* kv = (KeyValue*) obj;
      Mem_free(kv->value);
    });
    Dictionary_free(pq->index);
  }
}


size_t PriorityQueue_size(PriorityQueue* pq) {
  return pq->size;
}

static int pq_key_cmp(PriorityQueue* pq, double p1, double p2) {
  if(pq->order == PQOrder_ascending) {
    return p1 > p2;
  } else {
    return p1 < p2;
  }
}

static void PriorityQueue_try_realloc(PriorityQueue* pq) {
  if(pq->capacity > pq->size) {
    return;
  }

  pq->capacity *= 2;
  pq->array = (PQElem*)Mem_realloc(pq->array, sizeof(PQElem) * pq->capacity );
}

static void PriorityQueue_moveup(PriorityQueue* pq, size_t pos) {
  size_t parent = PriorityQueue_parent(pos);
  while(pos != 0 && pq_key_cmp(pq, pq->array[parent].priority, pq->array[pos].priority)) {
    PQElem_swap(pq, parent, pos);
    pos = parent;
    parent = PriorityQueue_parent(pos);
  }
}

static void PriorityQueue_movedown(PriorityQueue* pq, size_t pos) {
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
    PQElem_swap(pq, best, pos);
    PriorityQueue_movedown(pq, best);
  }
}

void PriorityQueue_push(PriorityQueue* pq, void* elem, double priority) {
  PriorityQueue_try_realloc(pq);

  pq->array[pq->size].elem = elem;
  pq->array[pq->size].priority = priority;

  if(pq->index) {
    size_t* index_value = Mem_alloc(sizeof(size_t));
    *index_value = pq->size;
    Dictionary_set(pq->index, elem, index_value);
  }

  PriorityQueue_moveup(pq, pq->size);
  pq->size += 1;
}

int PriorityQueue_empty(PriorityQueue* pq)  {
  return pq->size == 0;
}

void*  PriorityQueue_top_value(PriorityQueue* pq) {
  return pq->array[0].elem;
}

double PriorityQueue_top_priority(PriorityQueue* pq) {
  return pq->array[0].priority;
}

void PriorityQueue_pop(PriorityQueue* pq) {
  pq->array[0] = pq->array[pq->size-1];
  pq->size -= 1;
  PriorityQueue_movedown(pq, 0);
}

static size_t PriorityQueue_search_index(PriorityQueue* pq, void* elem) {
  if(pq->index) {
    size_t* index_value;
    if(!Dictionary_get(pq->index, elem, (void**)&index_value)) {
      return pq->size;
    }

    return *index_value;
  }

  size_t i;
  for(i=0; i<pq->size; ++i) {
    if(pq->array[i].elem == elem) {
      return i;
    }
  }

  return pq->size;
}

int PriorityQueue_get_priority(PriorityQueue* pq, void* elem, double* result) {
  size_t index_value = PriorityQueue_search_index(pq, elem);
  if(index_value == pq->size) {
    return 0;
  }

  *result = pq->array[index_value].priority;
  return 1;
}


void PriorityQueue_decrease_priority(PriorityQueue* pq, void* elem, double priority) {
  size_t i = PriorityQueue_search_index(pq, elem);

  if(i==pq->size) {
    Error_raise(Error_new(ERROR_GENERIC, "Priority queues: cannot find element while decreasing its priority"));
  }

  if(pq->array[i].priority < priority) {
    Error_raise(Error_new(ERROR_GENERIC, "Priority queues: the new priority is larger than the previous one in call to `decrease priority`"));
  }

  pq->array[i].priority = priority;
  PriorityQueue_moveup(pq, i);
}



int PriorityQueue_try_decrease_priority(PriorityQueue* pq, void* elem, double priority) {
  size_t i = PriorityQueue_search_index(pq, elem);

  if(i==pq->size) {
    return 0;
  }

  if(priority < pq->array[i].priority) {
    pq->array[i].priority = priority;
    PriorityQueue_moveup(pq, i);
    return 1;
  }

  return 0;
}


void PriorityQueue_foreach(PriorityQueue* pq, void (^callback)(void* elem, double priority)) {
  for(size_t i=0; i<pq->size; ++i) {
    callback(pq->array[i].elem, pq->array[i].priority);
  }
}
