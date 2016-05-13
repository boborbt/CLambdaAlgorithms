#pragma once

#include <stdlib.h>

typedef int (*MSCompareFun)(const void*, const void*);

void merge_sort(void** array, size_t count, MSCompareFun compare);
void merge_sort_g(void* array, size_t count, size_t size, MSCompareFun);
