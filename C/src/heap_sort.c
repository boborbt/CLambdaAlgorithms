#include "heap_sort.h"
#include <stdio.h>

static void swap(void** e1, void** e2) {
  void* tmp = *e1;
  *e1 = *e2;
  *e2 = tmp;
}

static long int parent(size_t index) {
  if(index==0) {
    return -1;
  }

  return (index-1) / 2;
}

static size_t left_child(size_t index) {
  return index * 2 + 1;
}

static size_t right_child(size_t index) {
  return index * 2 + 2;
}

static void move_down(void** array, size_t start, size_t end, KIComparator compare) {
  size_t root = start;
  size_t left = left_child(root);
  while( left <= end ) {
    size_t swap_index = root;
    if( compare(array[swap_index], array[left]) < 0) {
      swap_index = left;
    }

    size_t right = right_child(root);
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

static void heapify(void** array, size_t count, KIComparator compare) {
  long int start = parent(count-1);
  while(start >= 0) {
    move_down(array, (size_t) start, count-1,   compare);
    start-=1;
  }
}

void heap_sort(void** array, size_t count, KIComparator compare) {
  if(count == 0) {
    return;
  }
  heapify(array, count, compare);

  while(count>1) {
    swap(&array[0], &array[count-1]);
    move_down(array, 0, count-2, compare);
    count-=1;
  }
}
