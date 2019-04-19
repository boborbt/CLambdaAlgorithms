#pragma once

#include "iterator.h"
#include "array.h"

// --------------------------------------------------------------------------------
// Functions supported by all iterators
// --------------------------------------------------------------------------------

// Returns the first element returned by the iterator
void* first(Iterator it);

// Calls callback on each element of the container iterated by the given Iterator
void for_each(Iterator, void (^callback)(void* elem));

// Builds a new array from the elements accessible through
// the given iterator. In the results will be placed only those
// elements for which the given callback returns 1.
// Note: The elements are not duplicated so the elements contained
//   in the result should *not* be freed (unless the user needs that
//   for some reason).
Array* filter(Iterator, int (^)(void*));

// Calls callback on each element of the container iterated by the given Iterator
// it passes to the callback the current element and the current index.
void for_each_with_index(Iterator it, void(^callback)(void*, size_t));

// Finds the first element matching the condition evaluated by the callback
// `condition` must evaluate to 1 if the given elem is the one sought.
// it must evaluate to 0 otherwise.
// It returns NULL if no element satisfying the given condition is found.
void* find_first(Iterator, int(^condition)(void* elem));

// Use the given function pointer to map into a new object each object in the container iterated by
// the given iterator. It returns the array containing the mapped objects. The memory of the
// returned Array needs to be freed by the user.
Array* map(Iterator, void* (^)(void*));

// Frees the content of the given iterator using Mem_free
void free_contents(Iterator);

// --------------------------------------------------------------------------------
// Functions supported by bidirectional iterators
// --------------------------------------------------------------------------------

// Reverses the direction of the given iterator. I.e., it swaps the next and prev 
// functions.
Iterator reverse(Iterator);

// Finds the last element matching the condition evaluated by the callback
// `condition` must evaluate to 1 if the given elem is the one sought.
// it must evaluate to 0 otherwise.
//
// Requires a bidirectional iterator
void* find_last(Iterator, int(^condition)(void* elem));

// Returns the last element returned by the iterator.
//
// Requires a bidirectional iterators
// It supports non-bidirectional iterators with O(n) time complexity.
void* last(Iterator it);

// --------------------------------------------------------------------------------
// Functions supported by random iterators
// --------------------------------------------------------------------------------

// Return the number of elements on which it iterates.
//
// Requires a random access iterator
// It supports non-random iterators with complexity O(n) time complexity.
size_t count(Iterator it);

// Finds an occurrence of the given elem. It returns the index of the found element or (size_t) -1.
//
// Requires a random access iterator over a sorted container.
size_t binsearch(Iterator it, const void* elem, int (^compare)(const void* lhs, const void* rhs));

// Finds an occurrence of the given elem. It returns the index of the nearest element found.
//
// Requires a random access iterator over a sorted container.
size_t binsearch_approx(Iterator it, const void *elem, int (^compare)(const void *lhs, const void *rhs));

    // --------------------------------------------------------------------------------
    // Functions supported by mutable iterators
    // --------------------------------------------------------------------------------

    // Replaces the objects in the container with those returned by the given
    // function.
    //
    // Requires a mutable iterator.
    void replace(Iterator it, void * (^)(void *));

// --------------------------------------------------------------------------------
// Functions supported by a combination of iterators
// --------------------------------------------------------------------------------

// Reverses the content of the given iterator.
//
// Requires a bidirectional mutable iterator.
void reverse_contents(Iterator it);

// Sorts the elements iterated by it according to the order induced by the compare function.
// The function requires "it" to be a mutable and cloning iterator.
// If "it" is also a random access iterator, the function switches to an implementation that
// uses less memory and runs faster.
void sort(Iterator it, int (^compare)(const void*, const void*));
