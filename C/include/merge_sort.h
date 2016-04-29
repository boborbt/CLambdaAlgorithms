#pragma once

typedef int (*MSCompareFun)(const void*, const void*);

void merge_sort(void** array, unsigned int count, MSCompareFun compare);
void merge_sort_g(void* array, unsigned int count, unsigned int size, MSCompareFun);
