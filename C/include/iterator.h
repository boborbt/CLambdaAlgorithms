#pragma once

#include <stdlib.h>

// The Iterator type is the basis for the cross-container support for iteration functions.
// Every container should implement a functions that builds an Iterator for the container.
// That functions should simply call Iterator_make in order to fill the details necessary
// to support iteration over that container.
//
// Once such a function exist the container gain access to all iteration functions declared
// in iterator_functions.h (e.g., for_each, map, find_first, ...).


// Iterator type declaration

typedef struct _Iterator Iterator;

struct _Iterator {
  // all iterators
  void* container;
  void* (*new_iterator)(void*);
  void  (*next)(void*);
  void* (*get)(void*);
  int   (*end)(void* it);
  void  (*free)(void*);

  // random access iterators
  void   (*move_to)(void*, size_t);
  size_t (*size)(void*);

  // bidirectional iterators
  void   (*prev)(void*);
  void   (*to_end)(void*);
};


// Input: the functions that collectively allows building an iterator,
// iterating on a container using it, and destroying it.
//
// Note: the name of the function ending with "make" is to denote that this is
// not a "new" function and hence the Iterator needs not to be free.
//
// Note: the iterator object referenced below is the particular iterator object needed to
//       iterate over that specific containers, it has nothing to do with the Iterator type.
//
// The information that need to be provided is:
// - container: a pointer ot the actual container on which one needs to iterate
// - new: a pointer to the function that creates the iterator. The function needs to
//        take the container as input and have to return the new iterator object as its output.
// - next: a pointer to a function that advances the iterator object to the next element. The
//         function takes an iterator object as input.
// - get: a pointer to a function that dereferences the iterator and returns the object pointed by
//        the iterator. It takes in input the iterator object.
// - end: a pointer to a function that takes in input the iterator object and returns true if
//        it has reached the end of the container and false otherwise.
// - free: a pointer to a function that takes in input the iterator object and deallocates it.

Iterator Iterator_make(
  void* container,
  void* (*_new)(void*),
  void  (*_next)(void*),
  void* (*_get)(void*),
  int   (*_end)(void* it),
  void  (*_free)(void*)
);

// RandomAccessIterator APIs
// A RandomAccessIterator supports additional functions over containers (e.g., binsearch)
// To build a RandomAccessIterator you are supposed to invoke Iterator_make to intialize the
// "standard" part of the iterator and then use it with `RandomAccessIterator_make` to fill-in
// the RandomAccessIterator APIs.
//
// Example:
// ```C
//  Iterator it = Iterator_make(...);
//  it = RandomAccessIterator_make(it, _my_move_to_fun, _my_size_fun);
// ```

Iterator RandomAccessIterator_make(
  Iterator iterator,
  void   (*move_to)(void*, size_t),
  size_t (*size)(void*)
);

// BidirectionalIterator_make APIs
// A BidirectionalIterator supports additional functions over containers (e.g., iterating in reverse)
// order.
//
// To build a BidirectionalIterator you are supposed to invoke Iterator_make to intialize the
// "standard" part of the iterator and then use it with `RandomAccessIterator_make` to fill-in
// the RandomAccessIterator APIs.
//
// Example:
// ```C
//  Iterator it = Iterator_make(...);
//  it = BidirectionalIterator(it, _my_prev_fun, _my_to_end_fun);
// ```


Iterator BidirectionalIterator_make(
  Iterator iterator,
  void  (*prev)(void*),
  void  (*to_end)(void*)
);
