#pragma once

#include <stdlib.h>
#include "iterator.h"

// ArrayAlt implements a slightly modified interface of the Array
// library. ArrayAlt is almost never a better choice than Array.
// In fact, it is more inconvenient to use and more error prone.
// The difference between Array and ArrayAlt is that ArrayAlt stores
// objects in a continuous region of memory. This has two advantages:
// 1. It allows for faster performances when you need to iterate over a
//    huge list of value types objects, since it has better locality
//    properties than the Array implementation.
// 2. It allows to store value types and handles memory for them.
// If you are not in much need of either point 1 or 2, then it is almost
// always the case that you are better served by Array.

typedef struct _ArrayAlt ArrayAlt;
typedef struct _ArrayAltIterator ArrayAltIterator;

// Constructors
ArrayAlt* ArrayAlt_new(size_t capacity, size_t elem_size);
ArrayAlt* ArrayAlt_new_by_copying_carray(void* array, size_t size, size_t elem_size );
ArrayAlt* ArrayAlt_dup(ArrayAlt*);

// Destructor
void ArrayAlt_free(ArrayAlt*);

//
// Accessors
//

// Returns a pointer to the object at the given index. It raises an ERROR_INDEX_OUT_OF_BOUND
// if index is not in bounds. Bounds are [0, ArrayAlt_size(array)-1]
// Notice that the function does not return the object given input to the _add function,
// but a pointer to it. For instance, if the array contains Record objects, then the result
// will be of type Record* and a cast and a dereference is needed to get back the inserted record.
void* ArrayAlt_at(ArrayAlt* array, size_t index);

// Returns a C array representation of the given array. Valid indices are
// between 0 and ArrayAlt_size(array)-1.
// Note: due to the way the objects are stored into the array it is very seldom the
// case that this representation is useful to the user.
void* ArrayAlt_carray(ArrayAlt* array);

// Returns the number of elements in the current array
size_t ArrayAlt_size(ArrayAlt* array);

// Returns true iff ArrayAlt_size(array)==0
int ArrayAlt_empty(ArrayAlt* array);

// Returns the size in bytes of an element stored in the array
size_t ArrayAlt_elem_size(ArrayAlt* array);

// Returns the number of elements the array can accomodate without reallocating
// to a larger structure.
size_t ArrayAlt_capacity(ArrayAlt* array);

// Set the size the array to the new size reallocing the elements if necessary
void ArrayAlt_set_size(ArrayAlt* array, size_t new_size);

// semantically equivalent to array[index] = elem (if array was a carray)
// returns elem
void* ArrayAlt_set(ArrayAlt* array, size_t index, void* elem);


// inserts elem at the given index and shifts all elements up one position.
// it reallocates the array space if necessary
void ArrayAlt_insert(ArrayAlt*, size_t index, void* elem);

// appends elem to the given array. Notice that the given object needs to be
// a pointer to the actual object to be inserted. For instance, if the array
// needs to contains Record objects (and, thus, it has been created with
// ArrayAlt_new(capacity, sizeof(Record))), you need to give to the _add function
// pointers to Records, as in:
// ArrayAlt_add(array, &my_record);
void ArrayAlt_add(ArrayAlt* array, void* elem);

// removes an element at a specific index from the array and shifts all
// following elements down one position
void ArrayAlt_remove(ArrayAlt* array, size_t index);


// sorts in-place the given array. The parameters to the compare function
// are pointers to the objects inserted into the array. This implies that
// it is usually necessary cast and a dereference to get the actual value:
// e.g., if the array contains Record objects then the compare function
//   should be something like:
//
// int compare(const void* o1, const void* o2) {
//   Record rec1 = *(Record*) o1;
//   Record rec2 = *(Record*) o2;
//
//   ... do the comparison ...
// }
//
//
void ArrayAlt_sort(ArrayAlt* array, int (*compare)(const void*, const void*));
// void ArrayAlt_sort_f(ArrayAlt* array, void (*compare)(void*,void*));


// Iterator
ArrayAltIterator* ArrayAltIterator_new(ArrayAlt*);
void ArrayAltIterator_free(ArrayAltIterator*);

// Move the iterator to the next element. Do nothing if it is already past the
// end of the container.
void ArrayAltIterator_next(ArrayAltIterator* it);

// Returns 1 if the iterator is past the end of the container (i.e., if
// ArrayAltIterator_get would not return a sensible result), 0 otherwise.
int ArrayAltIterator_end(ArrayAltIterator* it);

// Returns the element currently pointed by the iterator
void* ArrayAltIterator_get(ArrayAltIterator* it);


// Returns 1 if it1 and it2 points to the same element.
int ArrayAltIterator_same(ArrayAltIterator* it1, ArrayAltIterator* it2);

//
// Iterator
//

Iterator ArrayAlt_it(ArrayAlt* array);
