#pragma once

#include <stdlib.h>


typedef int (*KIComparator)(const void*, const void*);

void insertion_sort(void** array, size_t count, KIComparator compare);
