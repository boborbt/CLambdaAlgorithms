#pragma once

typedef int (*ISCompareFun)(const void*, const void*);

void insertion_sort(void** array, unsigned int count, ISCompareFun compare);
