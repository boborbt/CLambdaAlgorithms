#pragma once

#include <stdlib.h>
#include "keys.h"

typedef int (*KIComparator)(const void*, const void*);

void merge_sort(void** array, size_t count, KIComparator compare);
void merge_sort_g(void* array, size_t count, size_t size, KIComparator);
