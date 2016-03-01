#include "insertion_sort.h"
#include <stdio.h>

void insert(const void** array, int elem_pos, ISCompareFun compare) {
  int i=elem_pos;
  const void* elem = array[elem_pos];
  for(; i-1>=0 && compare(elem, array[i-1]) < 0; --i) {
    array[i] = array[i-1];
  }

  array[i] = elem;
}


void insertion_sort(const void** array, int count, ISCompareFun compare) {
  for(int i=1; i<count; ++i) {
    if(i%100000 == 0) {
      printf(".");
      fflush(stdout);
    }
    insert(array, i, compare);
  }
  printf("\n");
}
