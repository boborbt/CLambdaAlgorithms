#include <stdlib.h>
#include <stdio.h>
#include "quick_sort.h"

int min(int e1, int e2) {
  return e1 < e2 ? e1 : e2;
}

int random_int(int size) {
  return drand48() * size;
}

void swap(const void** e1, const void** e2) {
  const void* tmp = *e1;
  *e1 = *e2;
  *e2 = tmp;
}

void array_block_swap_to_end(const void** array, int start, int end, int len) {
  int b1_start = start;
  int b2_start = end - len + 1;
  for(int i=0; i<len; ++i) {
    swap(&array[b1_start+i], &array[b2_start+i]);
  }
}

void dump_status(const void** array, int start, int end, int i, int j, int p) {
#ifdef __DEBUG__
  for(int k=start; k<=end; ++k) {
    printf("%3ld", (long) array[k]);
  }


  printf("\ni:%2d j:%2d p:%2d\n", i, j, p);
#endif
}

// Partition the given array putting all pivot elements in the center of it.
// After the call *p1 will be the index of the last array cell whose content
// is less than the pivot, and *p2 will be the first one larger than the pivot.
//
// The function is based on the following invariant:
// i is the last index of elements less than the pivot
// p is the first index of elements equal to the pivot
// all indices between i and p contain elements larger than the pivot
//
// Given this invariant, at the end of the work all pivots will be accumulated
// at the end of the array. Last step in the procedure swap those values
// with the first elements larger than the pivot.

void partition_3_way(const void** array, int start, int end, int pivot_pos, int* p1, int* p2,  QSCompareFun compare) {
  swap(&array[end], &array[pivot_pos]);
  const void* pivot = array[end];

  int i = start-1;
  int p = end;
  for(int j=start; j<p;) {
    if(compare(array[j], pivot)==0) {
      p-=1;
      swap(&array[j], &array[p]);
      continue;
    }

    if(compare(array[j], pivot) < 0) {
      ++i;
      swap(&array[i], &array[j]);
    }

    ++j;
  }

  int l = min(p-1-i, end-p+1);
  array_block_swap_to_end(array, i+1, end, l);

  *p1 = i;

  if(l == p-1-i) {
    *p2 = end-l+1;
  } else {
    *p2 = i+l+1;
  }
}


int partition(const void** array, int start, int end, QSCompareFun compare) {
  int pivot_pos = start + random_int(end-start);
  const void* pivot = array[end];
  swap( &array[end], &array[pivot_pos]);

  int i = start;
  for(int j=start; j <= end - 1; ++j) {
    if(compare(array[j], pivot) < 0) {
      swap(&array[i], &array[j]);
      ++i;
    }
  }

  swap(&array[i], &array[end]);
  return i;
}

void quick_sort_standard(const void** array, int start, int end, QSCompareFun compare) {
  if( end - start < 1 ) return;

  int pivot_pos = partition(array, start, end, compare);

  quick_sort_standard(array, start, pivot_pos, compare);
  quick_sort_standard(array, pivot_pos+1, end, compare );
}

void quick_sort_3_way(const void** array, int start, int end, QSCompareFun compare) {
  if(end - start < 1) return;

  int pivot_pos = start + random_int(end-start);
  int p1, p2;
  partition_3_way(array, start, end, pivot_pos, &p1, &p2, compare);

  quick_sort_3_way(array, start, p1, compare);
  quick_sort_3_way(array, p2, end, compare);
}


void quick_sort(const void** array, int count, QSCompareFun compare) {
  // quick_sort_standard(array, 0, count-1, compare);
  quick_sort_3_way(array, 0, count-1, compare);
}
