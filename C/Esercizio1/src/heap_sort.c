#include "heap_sort.h"
#include <stdio.h>

static void swap(void** e1, void** e2) {
  void* tmp = *e1;
  *e1 = *e2;
  *e2 = tmp;
}

static unsigned int parent(unsigned int index) {
  return (index-1) / 2;
}

static unsigned int left_child(unsigned int index) {
  return index * 2 + 1;
}

static unsigned int right_child(unsigned int index) {
  return index * 2 + 2;
}

static void move_down(void** array, unsigned int start, unsigned int end, HSCompareFun compare) {
  unsigned int root = start;
  unsigned int left = left_child(root);
  while( left <= end ) {
    unsigned int swap_index = root;
    if( compare(array[swap_index], array[left]) < 0) {
      swap_index = left;
    }

    unsigned int right = right_child(root);
    if(right <= end && compare(array[swap_index], array[right]) < 0) {
      swap_index = right;
    }

    if(swap_index == root) {
      return;
    }

    swap(&array[root], &array[swap_index]);
    root = swap_index;
    left = left_child(root);
  }
}

static void heapify(void** array, unsigned int count, HSCompareFun compare) {
  int start = (int) parent(count-1);
  while(start >= 0) {
    move_down(array, (unsigned int) start, count-1,   compare);
    start-=1;
  }
}

void heap_sort(void** array, unsigned int count, HSCompareFun compare) {
  heapify(array, count, compare);

  while(count>1) {
    swap(&array[0], &array[count-1]);
    move_down(array, 0, count-2, compare);
    count-=1;
  }
}
