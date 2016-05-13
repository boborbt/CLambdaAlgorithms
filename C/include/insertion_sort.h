#pragma once

#include <stdlib.h>


typedef int (*ISCompareFun)(const void*, const void*);

void insertion_sort(void** array, size_t count, ISCompareFun compare);
