#pragma once

#include <stdlib.h>

typedef struct _Stack* Stack;


Stack Stack_new(size_t capacity);
void Stack_free(Stack stack);

void* Stack_top(Stack stack);
void* Stack_pop(Stack stack);
void Stack_push(Stack stack, void* node);
int Stack_empty(Stack stack);
