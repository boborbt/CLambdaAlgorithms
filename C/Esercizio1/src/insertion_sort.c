#include "insertion_sort.h"
#include <stdio.h>

static void insert(void** array, unsigned int elem_pos, ISCompareFun compare) {
  unsigned int i=elem_pos;
  void* elem = array[elem_pos];
  for(; i>0 && compare(elem, array[i-1]) < 0; --i) {
    array[i] = array[i-1];
  }

  array[i] = elem;
}


void insertion_sort(void** array, unsigned int count, ISCompareFun compare) {
  for(unsigned int i=1; i<count; ++i) {
    insert(array, i, compare);
  }
}
