#ifndef __MERGE_SORT_H_J33NPL2934H
#define __MERGE_SORT_H_J33NPL2934H

typedef int (*MSCompareFun)(const void*, const void*);

void merge_sort(void** array, int count, MSCompareFun compare);

#endif
