#pragma once

#include <stdlib.h>

typedef int (*HSCompareFun)(const void*, const void*);

void heap_sort(void** array, size_t count, HSCompareFun compare);
