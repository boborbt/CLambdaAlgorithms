#pragma once

#include <stdlib.h>
#include "keys.h"

// Implements the merge sort algorithm
void merge_sort(void** array, size_t count, KIComparator compare);

// Implements the merge sort algorithm. This function allows one to sort arrays of any type
// whereas `merge_sort` assumes that each element of the array is a pointer.
void merge_sort_g(void* array, size_t count, size_t size, KIComparator);
