#include "heap_sort.h"
#include <stdio.h>

static void swap(const void** e1, const void** e2) {
  const void* tmp = *e1;
  *e1 = *e2;
  *e2 = tmp;
}

static int parent(int index) {
  return (index-1) / 2;
}

static int left_child(int index) {
  return index * 2 + 1;
}

static int right_child(int index) {
  return index * 2 + 2;
}

static void move_down(const void** array, int start, int end, HSCompareFun compare) {
  int root = start;
  int left = left_child(root);
  while( left <= end ) {
    int swap_index = root;
    if( compare(array[swap_index], array[left]) < 0) {
      swap_index = left;
    }

    int right = right_child(root);
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

static void heapify(const void** array, int count, HSCompareFun compare) {
  int start = parent(count-1);
  while(start >= 0) {
    move_down(array, start, count-1,   compare);
    start-=1;
  }
}

void heap_sort(const void** array, int count, HSCompareFun compare) {
  heapify(array, count, compare);

  while(count>1) {
    swap(&array[0], &array[count-1]);
    move_down(array, 0, count-2, compare);
    count-=1;
  }
}
