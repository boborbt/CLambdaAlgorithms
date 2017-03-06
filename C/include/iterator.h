#pragma once

#include <stdlib.h>

typedef struct _Iterator Iterator;

struct _Iterator {
  void* container;
  void*  (*new)(void*);
  void  (*next)(void*);
  void* (*get)(void*);
  int   (*end)(void* it);
  void  (*free)(void*);
};


// Input: the functions that collectively allows building an iterator,
// iterating on a container using it, and destroying it.
//
// Note: the name of the function ending with "make" is to denote that this is
// not a "new" function and hence the Iterator needs not to be freed
Iterator Iterator_make(
  void* container,
  void*  (*new)(void*),
  void  (*next)(void*),
  void* (*get)(void*),
  int   (*end)(void* it),
  void  (*free)(void*)
);

// Calls callback on each element of the container iterated by the given Iterator
void for_each(Iterator, void (^callback)(void* elem));
void for_each_with_index(Iterator it, void(^callback)(void*, size_t));

// Finds the first element matching the condition evaluated by the callback
// `condition` must evaluate to 1 if the given elem is the one sought.
// it must evaluate to 0 otherwise.
void* find_first(Iterator, int(^condition)(void* elem));
