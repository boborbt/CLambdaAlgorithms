#include "merge_sort.h"
#include <stdlib.h>
#include "array_g.h"

#include "mem.h"

static void merge(void** array, size_t start, size_t mid, size_t end, KIComparator compare) {
  void** buf = (void**) Mem_alloc(sizeof(const void*)*(end-start+1));
  size_t i = start;
  size_t j = mid+1;
  size_t k = 0;

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

  Mem_free(buf);
}

static void merge_g(void* array, size_t start, size_t mid, size_t end, size_t size, KIComparator compare) {
  void* buf = (void*) Mem_alloc(size*(end-start+1));
  size_t i = start;
  size_t j = mid+1;
  size_t k = 0;

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

  Mem_free(buf);
}

static void merge_sort_(void** array, size_t start, size_t end, KIComparator compare) {
  if(start >= end)
    return;

  size_t mid = (start + end)/2;

  merge_sort_(array, start, mid, compare);
  merge_sort_(array, mid+1, end, compare);
  merge(array, start, mid, end, compare);
}

static void merge_sort_g_(void* array, size_t start, size_t end, size_t size, KIComparator compare) {
  if(start >= end)
    return;

  size_t mid = (start + end)/2;

  merge_sort_g_(array, start, mid, size, compare);
  merge_sort_g_(array, mid+1, end, size, compare);
  merge_g(array, start, mid, end, size, compare);
}


void merge_sort_g(void* array, size_t count, size_t size, KIComparator compare) {
  if(count==0) {
    return;
  }
  merge_sort_g_(array, 0, count-1, size, compare);
}

void merge_sort(void** array, size_t count, KIComparator compare) {
  if(count == 0) {
    return;
  }
  merge_sort_(array, 0, count-1, compare);
}
