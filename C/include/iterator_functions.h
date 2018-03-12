#pragma once

#include "iterator.h"
#include "array.h"

// Sorts the elements iterated by it according to the order induced by the compare function.
// `it` is assumed to be a mutable iterator. This function will make an in-memory copy of your
// data into an Array. If your data is already in an array you might want to sort it using
// the Array_sort method.
void sort(Iterator it, int (^compare)(const void*, const void*));

// Return the number of elements on which it iterates. On random access iterators this function
// is O(1), otherwise it is O(n).
size_t count(Iterator it);

// Calls callback on each element of the container iterated by the given Iterator
void for_each(Iterator, void (^callback)(void* elem));

// Calls callback on each element of the container iterated by the given Iterator. The iterator
// must support the BidirectionalIterator APIs. The iteration will proceed in reverse order.
void for_each_reverse(Iterator, void (^callback)(void* elem));

// Calls callback on each element of the container iterated by the given Iterator
// it passes to the callback the current element and the current index.
void for_each_with_index(Iterator it, void(^callback)(void*, size_t));

// Finds the first element matching the condition evaluated by the callback
// `condition` must evaluate to 1 if the given elem is the one sought.
// it must evaluate to 0 otherwise.
void* find_first(Iterator, int(^condition)(void* elem));

// Finds the last element matching the condition evaluated by the callback
// `condition` must evaluate to 1 if the given elem is the one sought.
// it must evaluate to 0 otherwise.
// The given Iterator must implement the BidirectionalIterator APIs

void* find_last(Iterator, int(^condition)(void* elem));

// Finds an occurrence of the given elem. The iterator is assumed to be a random access iterator
// on a sorted container. It returns the index of the found element or (size_t) -1.
size_t binsearch(Iterator it, const void* elem, int (^compare)(const void* lhs, const void* rhs));

// Use the given function pointer to map into a new object each object in the container iterated by
// the given iterator. It returns the array containing the mapped objects. The memory of the
// returned Array needs to be freed by the user.
Array* map(Iterator, void* (^)(void*));

// Builds a new array from the elements accessible through
// the given iterator. In the results will be placed only those
// elements for which the given callback returns 1.
// Note: The elements are not duplicated so the elements contained
//   in the result should *not* be freed (unless the user needs that
//   for some reason).
Array* filter(Iterator, int (^)(void*));


// Reverses the content of the given iterator. The given iterator must be a bidirectional mutable
// iterator.
void reverse(Iterator it);

// Returns the first element returned by the iterator
void* first(Iterator it);

// Returns the last element returned by the iterator
// note: this is a O(n) operation unless the iterator implements the bidirectional iterator APIs,
//     (in such case it is a O(1) operation).
void* last(Iterator it);
