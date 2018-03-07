#pragma once

#include "keys.h"
#include <stdlib.h>

// Sorts the given array using the heap sort algorithm.
void heap_sort(void** array, size_t count, KIComparator compare);
