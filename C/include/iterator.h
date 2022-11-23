#pragma once

#include <stdlib.h>

/// @file iterator.h
/// @brief The Iterator type is the basis for the cross-container support for iteration functions.
///
/// # Iterator Interface
/// Every container should implement a function that builds an Iterator for the container.
/// That function should call Iterator_make in order to fill the details necessary
/// to support iteration over that container. Additional make functions are available to fill
/// the details for the different iterator traits: RandomAccessIterator_make, BidirectionalIterator_make,
/// MutableIterator_make, CloningIterator_make.
///
/// Once such a function exist, the container gains access to all iteration functions declared
/// in iterator_functions.h (e.g., for_each, map, find_first, ...).
///
///
/// ## Iterator type declaration
///
/// The Iterator type stores all the function pointers needed to work with the iterator APIs.
/// The Iterator struct can be actually thought as a meta-iterator interface since it will not
/// hold the actual iterators. Instead, it will provide means to create and work with iterators using
/// an interface that will be shared by all iterator implementations.
///
/// ### Basic Iterators
///
/// The information that need to create a new Iterator is:
/// - container: the actual container on which one needs to iterate
/// - new_iterator: a function that creates a new iterator instance.
///        input: the container on which to the iterator will operate;
///        output: a new iterator instance
/// - next: a function that advances the iterator object to the next element.
///        input: an iterator instance
/// - get: a function that dereferences the iterator and returns the object pointed by
///        the iterator.
///        input: the iterator instance
/// - end: a function that returns true if the iterator has become invalid;
///        input: the iterator instance
///        output: 1 if the iterator is no longer valid (it points either before the beginning or
///        past the end of the container)
/// - same: a function that returns 1 if the given two iterators point to the same
///        point in the same container and 0 otherwise
/// - free: a function that takes in input the iterator object and deallocates it.
///
/// ### Random Access Iterators
/// A random access iterator must also privide the following function pointers:
/// - move_to: a function that moves the iterator to a new place.
///       input: an instance of the iterator and the new position;
/// - size: a function returning the size of the container.
///       input: an instance of the iterator
///       output: the size of the underlying container
/// NOTE: move_to must be return a valid object every time it is called with a position
///       belonging to the range [0, size-1]
///
/// ### Bidirectional Iterators
/// A bidirectional iterator  must also privide the following function pointers:
///
/// - prev: a function that moves the iterator back of one position.
///        input: the iterator instance;
/// - to_end: a function that moves the iterator at the end of the container;
///        input: the iterator instance
/// 
/// ### Mutable Iterators
/// Mutable iterators  must also privide the following function pointers:
///
/// - set: a function that assign to the current pointed element the given value
///      input: an instance of the iterator and the value to be assigned
///
/// ### Cloning Iterators
/// Cloning iterators  must also privide the following function pointers:
///
///  - alloc_obj: a function that allocates enough memory to store a new instance of the objects
///       contained in the underlying container
///       input: an instance of the iterator
///       output: NULL if no memory needs to be alloced or the address to the alloced memory otherwise
///  - copy_obj: a function that stores the current object into the given memory and returns the given
///          given memory;
///       input: an interator instance and a pointer to the memory where to copy the data;
///       output: the pointer to the container object (if the container store pointers) or the pointer
///          received as input (in that case the function will have copied the current object into the
///          pointed memory)
///  - free_obj: frees the alloced memory (if necessary)
///      
/// NOTE: cloning is only useful for value types. When a container stores pointers, cloning can
/// (and for performance reasons should) degrade to simply returning the pointers stored in the
/// container. To understand why this is the case, consider that the client of the api will store
/// the result of the cloning operation into a pointer. Since that storage is sufficient to actually
/// hold the data (i.e., the pointer in the container) it is not necessary to alloc memory just to
/// hold the pointer.

typedef struct _Iterator Iterator;

struct _Iterator {
  // all iterators
  void* container;
  void* (*new_iterator)(void*);
  void  (*next)(void*);
  void* (*get)(void*);
  int   (*end)(void* it);
  int   (*same)(void* it1, void* it2);
  void  (*free)(void*);

  // random access iterators
  void   (*move_to)(void*, size_t);
  size_t (*size)(void*);

  // bidirectional iterators
  void   (*prev)(void*);
  void   (*to_begin)(void*);
  void   (*to_end)(void*);

  // mutable iterators
  void   (*set)(void*, void*);

  // cloning iterators
  void* (*alloc_obj)(void*);
  void* (*copy_obj)(void*, void*);
  void  (*free_obj)(void*);
};


// The following functions can be used to verify if an iterator satisfy a given piece of the
// Iterator interface.

/// @brief Returns 1 if the iterator is a bidirectional iterator and 0 otherwise.
int is_bidirectional_iterator(Iterator it);

/// @brief Returns 1 if the iterator is a random access iterator and 0 otherwise.
int is_random_access_iterator(Iterator it);

/// @brief Returns 1 if the iterator is a mutable iterator and 0 otherwise.
int is_mutable_iterator(Iterator it);

/// @brief Returns 1 if the iterator is a cloning iterator and 0 otherwise.
int is_cloning_iterator(Iterator it);

// The following functions can be used to generate an error and halt the program if an iterator
// does not satisfy a given piece of the Iterator interface.

/// @brief If the iterator is not a bidirectional iterator, it will generate an error and halt the program.
void require_bidirectional_iterator(Iterator it);

/// @brief If the iterator is not a random access iterator, it will generate an error and halt the program.
void require_random_access_iterator(Iterator it);

/// @brief If the iterator is not a mutable iterator, it will generate an error and halt the program.
void require_mutable_iterator(Iterator it);

/// @brief If the iterator is not a cloning iterator, it will generate an error and halt the program.
void require_cloning_iterator(Iterator it);




/// @brief Helper for making Iterators
///
/// Input: the functions that collectively allows building an iterator,
/// iterating on a container using it, and destroying it.
///
/// Note: the name of the function ending with "make" is to denote that this is
///        not a "new" function and hence the Iterator needs not to be free.
///
/// Note: the iterator object referenced below is the particular iterator object needed to
///       iterate over that specific containers, it has nothing to do with the Iterator type.


Iterator Iterator_make(
  void* container,
  void* (*_new)(void*),
  void  (*_next)(void*),
  void* (*_get)(void*),
  int   (*_end)(void* it),
  void  (*to_begin)(void*),
  int   (*_same)(void* it1, void* it2),
  void  (*_free)(void*)
);

/// @brief RandomAccessIterator APIs
/// A RandomAccessIterator supports additional functions over containers (e.g., binsearch)
/// To build a RandomAccessIterator you are supposed to invoke Iterator_make to intialize the
/// "standard" part of the iterator and then use it with `RandomAccessIterator_make` to fill-in
/// the RandomAccessIterator APIs.
///
/// Example:
/// ```C
///  Iterator it = Iterator_make(...);
///  it = RandomAccessIterator_make(it, _my_move_to_fun, _my_size_fun);
/// ```

Iterator RandomAccessIterator_make(
  Iterator iterator,
  void   (*move_to)(void*, size_t),
  size_t (*size)(void*)
);

/// @brief BidirectionalIterator APIs
/// A BidirectionalIterator supports additional functions over containers (e.g., iterating in reverse)
/// order.
///
/// To build a BidirectionalIterator you are supposed to invoke Iterator_make to intialize the
/// "standard" part of the iterator and then use it with `BidirectionalIterator_make` to fill-in
/// the BidirectionalIterator APIs.
///
/// Example:
/// ```C
///  Iterator it = Iterator_make(...);
///  it = BidirectionalIterator_make(it, _my_prev_fun _my_to_end_fun);
/// ```


Iterator BidirectionalIterator_make(
  Iterator iterator,
  void  (*prev)(void*),
  void  (*to_end)(void*)
);


/// @brief MutableIterator APIs
/// A mutable iterator supports the assignment of a new value to the iterated objects.
///
/// To build a MutableIterator you are supposed to invoke Iterator_make to intialize the
/// "standard" part of the iterator and then use it with `MutableIterator_make` to fill-in
/// the MutableIterator APIs.
///
/// Example:
/// ```C
///  Iterator it = Iterator_make(...);
///  it = MutableIterator_make(it, _my_set_fun);
/// ``

Iterator MutableIterator_make(
  Iterator iterator,
  void  (*set)(void*, void*)
);


/// @brief CloningIterator
/// A cloning iterator supports cloning of objects in the container being
/// iterated. See the description of the Iterator struct for important information about how to
/// implement the cloning API.
///
/// To build a CloningIterator you are supposed to invoke Iterator_make to intialize the
/// "standard" part of the iterator and then use it with `CloningIterator_make` to fill-in
/// the ClongingIterator APIs.
///

Iterator CloningIterator_make(
  Iterator iterator,
  void* (*alloc_obj)(void* iterator),
  void* (*copy_obj)(void* iterator, void* to_mem),
  void (*free_obj)(void* obj)
);
