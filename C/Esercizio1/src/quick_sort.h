#pragma once

typedef int (*QSCompareFun)(const void*, const void*);

void quick_sort(void** array, unsigned int count, QSCompareFun compare);

void quick_sort_g(void* array, unsigned int count, unsigned int size, QSCompareFun);
