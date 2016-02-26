#ifndef __HEAP_SORT_H_K3MJ3NCU32__
#define __HEAP_SORT_H_K3MJ3NCU32__

typedef int (*HSCompareFun)(const void*, const void*);

void heap_sort(const void** array, int count, HSCompareFun compare);
#endif
