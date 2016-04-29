#pragma once

typedef int (*HSCompareFun)(const void*, const void*);

void heap_sort(void** array, unsigned int count, HSCompareFun compare);
