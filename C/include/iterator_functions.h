#pragma once

#include "iterator.h"
#include "array.h"

// Calls callback on each element of the container iterated by the given Iterator
void for_each(Iterator, void (^callback)(void* elem));
void for_each_with_index(Iterator it, void(^callback)(void*, size_t));

// Finds the first element matching the condition evaluated by the callback
// `condition` must evaluate to 1 if the given elem is the one sought.
// it must evaluate to 0 otherwise.
void* find_first(Iterator, int(^condition)(void* elem));
Array* map(Iterator, void* (^)(void*));

// Builds a new array from the elements accessible through
// the given iterator. In the results will be placed only those
// elements for which the given callback returns 1.
// Note: The elements are not duplicated so the elements contained
//   in the result should *not* be freed (unless the user needs that
//   for some reason).
Array* filter(Iterator, int (^)(void*));

// Returns the first element returned by the iterator
void* first(Iterator it);

// Returns the last element returned by the iterator (note: this
// is a O(n) operation)
void* last(Iterator it);
