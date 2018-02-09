#pragma once

#include <stdlib.h>

typedef struct _Iterator Iterator;

struct _Iterator {
  void* container;
  void* (*new)(void*);
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
  void* (*new)(void*),
  void  (*next)(void*),
  void* (*get)(void*),
  int   (*end)(void* it),
  void  (*free)(void*)
);
