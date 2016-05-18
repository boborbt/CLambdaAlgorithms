#include "insertion_sort.h"
#include <stdio.h>

static void insert(void** array, size_t elem_pos, KIComparator compare) {
  size_t i=elem_pos;
  void* elem = array[elem_pos];
  for(; i>0 && compare(elem, array[i-1]) < 0; --i) {
    array[i] = array[i-1];
  }

  array[i] = elem;
}


void insertion_sort(void** array, size_t count, KIComparator compare) {
  for(size_t i=1; i<count; ++i) {
    insert(array, i, compare);
  }
}
