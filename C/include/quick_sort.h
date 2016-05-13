#pragma once

typedef int (*QSCompareFun)(const void*, const void*);

void quick_sort(void** array, size_t count, QSCompareFun compare);

void quick_sort_g(void* array, size_t count, size_t size, QSCompareFun);
