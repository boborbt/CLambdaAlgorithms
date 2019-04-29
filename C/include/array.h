#pragma once

#include <stdlib.h>
#include "iterator.h"
#include "keys.h"

typedef struct _Array Array;
typedef struct _ArrayIterator ArrayIterator;

// Constructors
Array* Array_new(size_t capacity);
Array* Array_new_by_copying_carray(void* array, size_t size);
Array* Array_dup(Array*);

// Destructor
void Array_free(Array*);

//
// Accessors
//

// Returns 1 if a1 and a2 contains the same objects, 0 otherwise.
int Array_same(Array* a1, Array* a2);


// Returns a pointer to the object at the given index. It raises an ERROR_INDEX_OUT_OF_BOUND
// if index is not in bounds. Bounds are [0, Array_size(array)-1]
void* Array_at(Array* array, size_t index);

// Returns a C array representation of the given array. Valid indices are
// between 0 and Array_size(array)-1.
void* Array_carray(Array* array);

// Returns the number of elements in the current array
size_t Array_size(Array* array);

// Returns true iff Array_size(array)==0
int Array_empty(Array* array);

// Returns the size in bytes of an element stored in the array
size_t Array_elem_size(Array* array);

// Returns the number of elements the array can accomodate without reallocating
// to a larger structure.
size_t Array_capacity(Array* array);

// Set the size the array to the new size reallocing the elements if necessary
void Array_set_size(Array* array, size_t new_size);

// semantically equivalent to array[index] = elem (if array was a carray)
// returns elem
void* Array_set(Array* array, size_t index, void* elem);


// inserts elem at the given index and shifts all elements up one position.
// it reallocates the array space if necessary
void Array_insert(Array*, size_t index, void* elem);

// appends elem to the given array.
void Array_add(Array* array, void* elem);

// removes an element at a specific index from the array and shifts all
// following elements down one position
void Array_remove(Array* array, size_t index);


// sorts in-place the given array. The parameters to the compare function
// are pointers to the objects inserted into the array. This implies that
// it is usually necessary a cast and a dereference to get the actual value:
// e.g., if the array contains Record objects then the compare block
//   should be something like:
//
// int (^compare)(const void* o1, const void* o2) {
//   Record rec1 =  o1;
//   Record rec2 =  o2;
//
//   ... do the comparison ...
// }
// 
// the compare function should follow the strcmp pattern (0 means the two
// objects are equal, -1 means that the first smaller than the second, and
// +1 means that the first is larger than the second).
void Array_sort(Array* array, KIBlkComparator compare);

// Iterator
ArrayIterator* ArrayIterator_new(Array*);
void ArrayIterator_free(ArrayIterator*);

size_t ArrayIterator_size(ArrayIterator*);

// Move the iterator to the next element. Do nothing if it is already past the
// end of the container.
void ArrayIterator_next(ArrayIterator*);


// Move the iterator to the previous element. Do nothing if it is already past the
// end (i.e., beginning) of the container.
void ArrayIterator_prev(ArrayIterator*);

void ArrayIterator_move_to(ArrayIterator*, size_t);

// Moves the iterator to the begin of the container (i.e., on the first element)
void ArrayIterator_to_begin(ArrayIterator*);

// Moves the iterator to the end of the container (i.e., on the last element)
void ArrayIterator_to_end(ArrayIterator *);

// Returns 1 if the iterator is past the end of the container (i.e., if
// ArrayIterator_get would not return a sensible result), 0 otherwise.
int ArrayIterator_end(ArrayIterator* it);

// Returns the element currently pointed by the iterator
void* ArrayIterator_get(ArrayIterator* it);

// Sets the current pointed element to the given value
void ArrayIterator_set(ArrayIterator* it, void* value);

// Returns 1 if it1 and it2 points to the same element in the array, 0 otherwise.
int ArrayIterator_same(ArrayIterator* it1, ArrayIterator* it2);

//
// Iterator maker
// It returns a mutable bidirectional random iterator.

Iterator Array_it(Array* array);
