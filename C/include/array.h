#pragma once

#include <stdlib.h>
#include "iterator.h"
#include "keys.h"

/**
 * @file Array
 * @brief The Array structure implements a dynamic array and supports all iterator traits.
 *
 * The array can only contain pointers to data and it will not free the data when the array is freed.
 * 
 * **Example**
 *
 * ```c
 * #include "array.h"
 * #include "iterator_functions.h"
 * #include <stdio.h>
 * #include <stdlib.h>
 * 
 * int main() {
 *     // Create a new array with initial capacity of 10
 *     Array* array = Array_new(10);
 * 
 *     for (int i = 0; i < 10; i++) {
 *         int* val = malloc(sizeof(int));
 *         *val = i;
 *         Array_add(array, val);
 *     }
 * 
 *     for_each(Array_it(array), ^(void* element) {
 *         printf("%d ", *(int*)element);
 *     });
 * 
 *     for_each(Array_it(array), ^(void* element) {
 *         free(element);
 *     });
 * 
 *     Array_free(array);
 *     return 0;
 * }
 * ```
 */

typedef struct _Array Array;
typedef struct _ArrayIterator ArrayIterator;

// Constructors

/// @brief  Creates a new array.
/// @param capacity The initial capacity of the array.
/// @return A pointer to the newly created array.
Array* Array_new(size_t capacity);

/// @brief Creates a new array from an existing C array.
/// @param array the C array to copy.
/// @param size the size of the C array.
/// @return  A pointer to the newly created array.
Array* Array_new_by_copying_carray(void* array, size_t size);

/// @brief Creates a new array from an existing C array.
/// @param array the C array to copy.
/// @return  A pointer to the newly created array.
Array* Array_dup(Array*);

// Destructor

/// @brief Frees the memory allocated by the array.
/// @param array The array to free.
void Array_free(Array*);

//
// Accessors
//

/// @brief Returns 1 if a1 and a2 contains the same objects, 0 otherwise.
/// @param a1 The first array.
/// @param a2 The second array.
/// @return 1 if a1 and a2 contains the same objects, 0 otherwise. Equality is determined by pointer equality.
int Array_same(Array* a1, Array* a2);


/// @brief returns a pointer to the object at the given index. 
/// @param array The array.
/// @param index The index of the object to return.
/// @return A pointer to the object at the given index.
/// @warning The index must be within the bounds of the array. It raises an ERROR_INDEX_OUT_OF_BOUND if index is not in bounds. 
///          Bounds are [0, Array_size(array)-1]
void* Array_at(Array* array, size_t index);

/// @brief Returns a C array representation of the given array.
void* Array_carray(Array* array);

/// @brief Returns the number of elements in the current array
size_t Array_size(Array* array);

/// @brief Returns true iff Array_size(array)==0
int Array_empty(Array* array);

/// @brief Returns the size in bytes of an element stored in the array
size_t Array_elem_size(Array* array);

/// @brief Returns the number of elements the array can accomodate without reallocating
/// to a larger structure.
size_t Array_capacity(Array* array);

/// @brier Set the size the array to the new size reallocing the elements if necessary
void Array_set_size(Array* array, size_t new_size);

/// @brief semantically equivalent to array[index] = elem (if array was a carray)
/// @brief returns elem
void* Array_set(Array* array, size_t index, void* elem);


/// @brief inserts elem at the given index and shifts all elements up one position.
/// it reallocates the memory taken by the array if necessary
void Array_insert(Array*, size_t index, void* elem);

/// @brief appends elem to the given array.
/// it reallocates the memory taken by the array if necessary
void Array_add(Array* array, void* elem);

/// @brief removes an element at a specific index from the array and shifts all
/// following elements down one position
void Array_remove(Array* array, size_t index);


/// @brief sorts in-place the given array. 
/// The parameters to the compare function
/// are pointers to the objects inserted into the array. This implies that
/// it is usually necessary a cast and a dereference to get the actual value:
/// e.g., if the array contains Record objects then the compare block
///   should be something like:
/// ```c
/// int (^compare)(const void* o1, const void* o2) {
///   Record rec1 =  o1;
///   Record rec2 =  o2;
///
///   ... do the comparison ...
/// }
/// ```
/// the compare function should follow the strcmp pattern (0 means the two
/// objects are equal, -1 means that the first smaller than the second, and
/// +1 means that the first is larger than the second).
void Array_sort(Array* array, KIBlkComparator compare);

/// @brief Returns a new iterator over the array.
ArrayIterator* ArrayIterator_new(Array*);

/// @brief Frees the memory allocated by the iterator.
void ArrayIterator_free(ArrayIterator*);

/// @brief Returns the size of the array subject to iteration.
size_t ArrayIterator_size(ArrayIterator*);

/// @brief Move the iterator to the next element. Do nothing if it is already past the
/// end of the container.
void ArrayIterator_next(ArrayIterator*);


/// @brief Move the iterator to the previous element. Do nothing if it is already past the
/// end (i.e., beginning) of the container.
void ArrayIterator_prev(ArrayIterator*);

/// @brief Move the iterator to the specified index.
void ArrayIterator_move_to(ArrayIterator*, size_t);

/// @brief Moves the iterator to the begin of the container (i.e., on the first element)
void ArrayIterator_to_begin(ArrayIterator*);

/// @brief Moves the iterator to the end of the container (i.e., on the last element)
void ArrayIterator_to_end(ArrayIterator *);

/// @brief Returns 1 if the iterator is past the end of the container (i.e., if
/// ArrayIterator_get would not return a sensible result), 0 otherwise.
int ArrayIterator_end(ArrayIterator* it);

/// @brief Returns the element currently pointed by the iterator
void* ArrayIterator_get(ArrayIterator* it);

/// @brief Sets the current pointed element to the given value
void ArrayIterator_set(ArrayIterator* it, void* value);

/// @brief Returns 1 if it1 and it2 points to the same element in the array, 0 otherwise.
int ArrayIterator_same(ArrayIterator* it1, ArrayIterator* it2);


// Iterator maker
// It returns a mutable bidirectional random iterator.

/// @brief Creates a new Iterator interface to the iterators supported by the Array.
/// @param array 
/// @return an Iterator over the array.
Iterator Array_it(Array* array);
