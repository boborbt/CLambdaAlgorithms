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

// FIXME: TO BE IMPLEMENTED
void* reduce(Iterator, void* zero, void* (^)(void* accumulator, void* element) );

Array* filter(Iterator, size_t, int (^)(void*));
