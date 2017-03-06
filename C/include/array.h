#pragma once

#include <stdlib.h>
#include "iterator.h"


typedef struct _Array* Array;
typedef struct _ArrayIterator* ArrayIterator;


// Constructors
Array Array_new(size_t capacity, size_t elem_size);
Array Array_new_by_copying_carray(void* array, size_t size, size_t elem_size );
Array Array_dup(Array);

// Destructor
void Array_free(Array);

//
// Accessors
//

// Returns a pointer to the object at the given index. It raises an ERROR_INDEX_OUT_OF_BOUND
// if index is not in bounds. Bounds are [0, Array_size(array)-1]
// Notice that the function does not return the object given input to the _add function,
// but a pointer to it. For instance, if the array contains Record objects, then the result
// will be of type Record* and a cast and a dereference is needed to get back the inserted record.
void* Array_at(Array array, size_t index);

// Returns a C array representation of the given array. Valid indices are
// between 0 and Array_size(array)-1.
// Note: due to the way the objects are stored into the array it is very seldom the
// case that this representation is useful to the user.
void* Array_carray(Array array);

// Returns the number of elements in the current array
size_t Array_size(Array array);

// Returns true iff Array_size(array)==0
int Array_empty(Array array);

// Returns the size in bytes of an element stored in the array
size_t Array_elem_size(Array array);

// Returns the number of elements the array can accomodate without reallocating
// to a larger structure.
size_t Array_capacity(Array array);

// Set the size the array to the new size reallocing the elements if necessary
void Array_set_size(Array array, size_t new_size);

// semantically equivalent to array[index] = elem (if array was a carray)
// returns elem
void* Array_set(Array array, size_t index, void* elem);


// inserts elem at the given index and shifts all elements up one position.
// it reallocates the array space if necessary
void Array_insert(Array, size_t index, void* elem);

// appends elem to the given array. Notice that the given object needs to be
// a pointer to the actual object to be inserted. For instance, if the array
// needs to contains Record objects (and, thus, it has been created with
// Array_new(capacity, sizeof(Record))), you need to give to the _add function
// pointers to Records, as in:
// Array_add(array, &my_record);
void Array_add(Array array, void* elem);

// removes an element at a specific index from the array and shifts all
// following elements down one position
void Array_remove(Array array, size_t index);


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
void Array_sort(Array array, int (*compare)(const void*, const void*));
// void Array_sort_f(Array array, void (*compare)(void*,void*));


// Iterator
ArrayIterator ArrayIterator_new(Array);
void ArrayIterator_free(ArrayIterator);

// Move the iterator to the next element. Do nothing if it is already past the
// end of the container.
void ArrayIterator_next(ArrayIterator it);

// Returns 1 if the iterator is past the end of the container (i.e., if
// ArrayIterator_get would not return a sensible result), 0 otherwise.
int ArrayIterator_end(ArrayIterator it);

// Returns the element currently pointed by the iterator
void* ArrayIterator_get(ArrayIterator it);

//
// FOREACH
//

Iterator Array_it(Array array);
