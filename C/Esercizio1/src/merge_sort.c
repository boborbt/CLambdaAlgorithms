#include "merge_sort.h"
#include <stdlib.h>


void merge(const void** array, int start, int mid, int end, MSCompareFun compare) {
  const void** buf = (const void**) malloc(sizeof(const void*)*(end-start+1));
  int i = start;
  int j = mid+1;
  int k = 0;

  while(i<=mid && j<=end) {
    if(compare(array[i], array[j]) <= 0) {
      buf[k++] = array[i++];
    } else {
      buf[k++] = array[j++];
    }
  }

  while(i<=mid) {
    buf[k++] = array[i++];
  }

  while(j<=end) {
    buf[k++] = array[j++];
  }

  // copying result back into the array
  for(k=0; k<(end-start+1); ++k) {
    array[start+k] = buf[k];
  }

  free(buf);
}

void merge_sort_(const void** array, int start, int end, MSCompareFun compare) {
  if(start >= end)
    return;

  int mid = (start + end)/2;

  merge_sort_(array, start, mid, compare);
  merge_sort_(array, mid+1, end, compare);
  merge(array, start, mid, end, compare);
}


void merge_sort(const void** array, int count, MSCompareFun compare) {
  merge_sort_(array, 0, count-1, compare);
}
