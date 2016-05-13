#pragma once

#include <stdlib.h>


typedef struct _Array* Array;
typedef struct _ArrayIterator* ArrayIterator;


// Constructors
Array Array_new(size_t size, size_t elem_size);
Array Array_new_by_copying_carray(void* array, size_t size, size_t elem_size );

// Destructor
void Array_free(Array);

// Accessors
void* Array_at(Array array, size_t index);
void* Array_carray(Array array);
size_t Array_size(Array array);

// Set the size the array to the new size reallocing the elements if necessary
void Array_set_size(Array array, size_t new_size);

// semantically equivalent to array[index] = elem (if array was a carray)
void Array_set(Array array, size_t index, void* elem);


// inserts elem at the given index and shifts all elements up one position.
// it reallocates the array space if necessary
void Array_insert(Array, size_t index, void* elem);

// appends elem to the given array
void Array_add(Array array, void* elem);

// removes an element at a specific index from the array and shifts all
// following elements down one position
void Array_remove(Array array, size_t index);


// Iterator
ArrayIterator ArrayIterator_new(Array);
void ArrayIterator_free(ArrayIterator);

// Move the iterator to the next element. Do nothing if it is already past the
// end of the container.
void ArrayIterator_next(ArrayIterator it);

// Returns 1 if the iterator is past the end of the container (i.e., if
// ArrayIterator_get would return a sensible result), 0 otherwise.
int ArrayIterator_end(ArrayIterator it);

// Returns the element currently pointed by the iterator
void* ArrayIterator_get(ArrayIterator it);
