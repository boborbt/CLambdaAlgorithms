#pragma once

#include <stdlib.h>
#include "keys.h"

void merge_sort(void** array, size_t count, KIComparator compare);
void merge_sort_g(void* array, size_t count, size_t size, KIComparator);
