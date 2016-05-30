#pragma once

#include <stdlib.h>

// Stack opaque type
typedef struct _Stack* Stack;


// Alloc and initialize a new stck. The capacity is an hint about how large
// the stack is expected to grow. If the stack grows larger than capacity, the
// data structure is automatically realloced to accomodate new elements.
Stack Stack_new(size_t capacity);

// Frees the memory occupied by the stack. User objects contained in the stack
// are left untouched.
void Stack_free(Stack stack);

// Returns a pointer to object at the top of the stack.
void* Stack_top(Stack stack);

// Returns a pointer to object at the top of the stack and removes it.
void* Stack_pop(Stack stack);

// Add an object at the top of the stack.
void Stack_push(Stack stack, void* node);

// Returns 1 if the stack is empty, 0 otherwise.
int Stack_empty(Stack stack);
