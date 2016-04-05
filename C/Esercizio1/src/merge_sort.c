#include "merge_sort.h"
#include <stdlib.h>
#include "array_g.h"

static void merge(void** array, unsigned int start, unsigned int mid, unsigned int end, MSCompareFun compare) {
  void** buf = (void**) malloc(sizeof(const void*)*(end-start+1));
  unsigned int i = start;
  unsigned int j = mid+1;
  unsigned int k = 0;

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

static void merge_g(void* array, unsigned int start, unsigned int mid, unsigned int end, unsigned int size, MSCompareFun compare) {
  void* buf = (void*) malloc(size*(end-start+1));
  unsigned int i = start;
  unsigned int j = mid+1;
  unsigned int k = 0;

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

static void merge_sort_(void** array, unsigned int start, unsigned int end, MSCompareFun compare) {
  if(start >= end)
    return;

  unsigned int mid = (start + end)/2;

  merge_sort_(array, start, mid, compare);
  merge_sort_(array, mid+1, end, compare);
  merge(array, start, mid, end, compare);
}

static void merge_sort_g_(void* array, unsigned int start, unsigned int end, unsigned int size, MSCompareFun compare) {
  if(start >= end)
    return;

  unsigned int mid = (start + end)/2;

  merge_sort_g_(array, start, mid, size, compare);
  merge_sort_g_(array, mid+1, end, size, compare);
  merge_g(array, start, mid, end, size, compare);
}


void merge_sort_g(void* array, unsigned int count, unsigned int size, MSCompareFun compare) {
  if(count==0) {
    return;
  }
  merge_sort_g_(array, 0, count-1, size, compare);
}

void merge_sort(void** array, unsigned int count, MSCompareFun compare) {
  if(count == 0) {
    return;
  }
  merge_sort_(array, 0, count-1, compare);
}
