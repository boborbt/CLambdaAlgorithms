#include "stack.h"
#include <stdlib.h>
#include <stdio.h>

struct _Stack {
  void** array;
  unsigned int size;
  unsigned int capacity;
};


/* --------------------------
 * Stack implementation
 * -------------------------- */

void* Stack_top(Stack stack) {
  if(stack->size==0) {
    return NULL;
  }

  return stack->array[stack->size-1];
}

void* Stack_pop(Stack stack) {
  if(stack->size==0) {
    return NULL;
  }

  void* tmp = stack->array[--stack->size];
  return tmp;
}

void Stack_push(Stack stack, void* node) {
  if(stack->size >= stack->capacity) {
    printf("Stack size exceeded");
    exit(1);
  }

  stack->array[stack->size++] = node;
}

int Stack_empty(Stack stack) {
  return stack->size == 0;
}

Stack Stack_new(unsigned int capacity) {
  Stack result = (Stack) malloc(sizeof(struct _Stack));
  result->array = (void**) malloc(sizeof(void*) * capacity);
  result->size = 0;
  result->capacity = capacity;
  return result;
}

void Stack_free(Stack stack) {
  free(stack->array);
  free(stack);
}
