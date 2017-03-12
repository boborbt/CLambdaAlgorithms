#include "stack.h"
#include "array.h"
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"

#include "mem.h"

struct _Stack {
  Array* array;
};


/* --------------------------
 * Stack* implementation
 * -------------------------- */

void* Stack_top(Stack* stack) {
  if(Array_size(stack->array)==0) {
    return NULL;
  }

  return Array_at(stack->array, Array_size(stack->array)-1);
}

void* Stack_pop(Stack* stack) {
  if(Array_size(stack->array)==0) {
    return NULL;
  }

  size_t last_index = Array_size(stack->array)-1;
  void* tmp = Array_at(stack->array, last_index);
  Array_remove(stack->array, last_index);
  return tmp;
}

void Stack_push(Stack* stack, void* node) {
  Array_add(stack->array, node);
}

int Stack_empty(Stack* stack) {
  return Array_empty(stack->array);
}

Stack* Stack_new(size_t capacity) {
  Stack* result = (Stack*) Mem_alloc(sizeof(struct _Stack));
  result->array = Array_new(capacity);
  return result;
}

void Stack_free(Stack* stack) {
  Array_free(stack->array);
  Mem_free(stack);
}
