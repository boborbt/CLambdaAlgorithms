#pragma once

#include <stdlib.h>
#include "keys.h"

// Sorts the given array using the insertion sort algorithm
void insertion_sort(void** array, size_t count, KIComparator compare);
