#pragma once

#include "keys.h"

// Implements the quick sort algorithm. It assumes that each element in the array is a pointer
// type.
void quick_sort(void** array, size_t count, KIComparator compare);

// Implements the quick sort algorithm. It assumes that each element in the array is a pointer
// type. 
void quick_sort_wb(void** array, size_t count, KIBlkComparator compare);

// Implements the quick sort algorithm. This function allows one to sort arrays of any type
// whereas `quick_sort` assumes that each element of the array is a pointer.
void quick_sort_g(void* array, size_t count, size_t size, KIComparator);
