#include "merge_sort.h"
#include <stdlib.h>
#include "array_g.h"


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

void merge_g(const void* array, int start, int mid, int end, int size, MSCompareFun compare) {
  void* buf = (void*) malloc(size*(end-start+1));
  int i = start;
  int j = mid+1;
  int k = 0;

  while(i<=mid && j<=end) {
    if(compare(at_g(array, i, size), at_g(array,j, size)) <= 0) {
      cp_g(at_g(buf, k++, size), at_g(array, i++, size), size);
    } else {
      cp_g(at_g(buf, k++, size), at_g(array, j++, size), size);
    }
  }

  while(i<=mid) {
    cp_g(at_g(buf, k++, size), at_g(array, i++, size), size);
  }

  while(j<=end) {
    cp_g(at_g(buf, k++, size), at_g(array, j++, size), size);
  }

  // copying result back into the array
  for(k=0; k<(end-start+1); ++k) {
    cp_g(at_g(array, start+k, size), at_g(buf, k, size), size);
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

void merge_sort_g_(const void* array, int start, int end, int size, MSCompareFun compare) {
  if(start >= end)
    return;

  int mid = (start + end)/2;

  merge_sort_g_(array, start, mid, size, compare);
  merge_sort_g_(array, mid+1, end, size, compare);
  merge_g(array, start, mid, end, size, compare);
}


void merge_sort_g(const void* array, int count, int size, MSCompareFun compare) {
  merge_sort_g_(array, 0, count-1, size, compare);
}

void merge_sort(const void** array, int count, MSCompareFun compare) {
  merge_sort_(array, 0, count-1, compare);
}
