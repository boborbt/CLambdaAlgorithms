#ifndef __INSERTION_SORT_H_JDU32MX2J__
#define __INSERTION_SORT_H_JDU32MX2J__

typedef int (*ISCompareFun)(const void*, const void*);

void insertion_sort(const void** array, int count, ISCompareFun compare);

#endif
