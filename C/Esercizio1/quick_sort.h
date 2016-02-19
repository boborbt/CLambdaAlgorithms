#ifndef __QUICK_SORT_C_KDFAJFDLAKENX__
#define __QUICK_SORT_C_KDFAJFDLAKENX__

typedef int (*QSCompareFun)(const void*, const void*);

void quick_sort(void** array, int count, QSCompareFun compare);

#endif
