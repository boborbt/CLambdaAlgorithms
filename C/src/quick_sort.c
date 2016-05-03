#include <stdlib.h>
#include <stdio.h>
#include "quick_sort.h"
#include "array_g.h"

void partition_3_way(void** array, unsigned int start, unsigned int end, unsigned int pivot_pos, unsigned int* p1, unsigned int* p2,  QSCompareFun compare);
void quick_sort_3_way(void** array, unsigned int start, unsigned int end, QSCompareFun compare);
void quick_sort_3_way_g(void* array, unsigned int start, unsigned int end, unsigned int size, QSCompareFun compare);
void quick_sort_standard(void** array, unsigned int start, unsigned int end, QSCompareFun compare);

static unsigned int umin(unsigned int e1, unsigned int e2) {
  return e1 < e2 ? e1 : e2;
}

static unsigned int random_int(unsigned int size) {
  return (unsigned int) (drand48() * size);
}

static void swap(void** e1,void** e2) {
  void* tmp = *e1;
  *e1 = *e2;
  *e2 = tmp;
}



static void array_block_swap_to_end(void** array, unsigned int start, unsigned int end, unsigned int len) {
  unsigned int b1_start = start;
  unsigned int b2_start = (unsigned int) (end - len + 1);
  for(unsigned int i=0; i<len; ++i) {
    swap(&array[b1_start+i], &array[b2_start+i]);
  }
}

static void array_block_swap_to_end_g(void* array, unsigned int start, unsigned int end, unsigned int len, unsigned int size) {
  unsigned int b1_start = start;
  unsigned int b2_start = (unsigned int)(end - len + 1);
  for(unsigned int i=0; i<len; ++i) {
    swap_g(at_g(array, b1_start+i,size),
           at_g(array, b2_start+i,size),
           size);
  }
}

#ifdef __DEBUG__
static void dump_status(const void** array, unsigned int start, unsigned int end, int i, int j, int p) {
  for(int k=start; k<=end; ++k) {
    printf("%3ld", (long) array[k]);
  }


  printf("\ni:%2d j:%2d p:%2d\n", i, j, p);
}
#endif

// Partition the given array putting all pivot elements in the center of it.
// After the call *p1 will be the index of the last array cell whose content
// is less than the pivot, and *p2 will be the first one larger than the pivot.
//
// The function is based on the following invariant:
// i is the last index of elements less than the pivot
// p is the first index of elements equal to the pivot
// all indices between i and p contain elements larger than the pivot
//
// Given this invariant, at the end of the work all pivots will be accumulated
// at the end of the array. Last step in the procedure swap those values
// with the first elements larger than the pivot.

void partition_3_way(void** array, unsigned int start, unsigned int end, unsigned int pivot_pos, unsigned int* p1, unsigned int* p2,  QSCompareFun compare) {
  swap(&array[end], &array[pivot_pos]);
  const void* pivot = array[end];

  unsigned int i = (unsigned int)(start-1);
  unsigned int p = end;
  for(unsigned int j=start; j<p;) {
    if(compare(array[j], pivot)==0) {
      p-=1;
      swap(&array[j], &array[p]);
      continue;
    }

    if(compare(array[j], pivot) < 0) {
      ++i;
      swap(&array[i], &array[j]);
    }

    ++j;
  }

  unsigned int l = umin(p-1-i, end-p+1);
  array_block_swap_to_end(array, i+1, end, l);

  if(i==(unsigned int)-1) {
    *p1 = 0;
  } else {
    *p1 = i;
  }

  if(l == p-1-i) {
    *p2 = end-l+1;
  } else {
    *p2 = i+l+1;
  }
}

static void partition_3_way_g(void* array, unsigned int start, unsigned int end, unsigned int pivot_pos, unsigned int* p1, unsigned int* p2, unsigned int size, QSCompareFun compare) {
  swap_g(at_g(array,end,size), at_g(array,pivot_pos, size), size);
  const void* pivot_ptr = at_g(array,end,size);

  unsigned int i = start-1;
  unsigned int p = end;
  for(unsigned int j=start; j<p;) {
    if(compare(at_g(array,j,size), pivot_ptr)==0) {
      p-=1;
      swap_g(at_g(array,j,size), at_g(array,p,size), size);
      continue;
    }

    if(compare(at_g(array,j,size), pivot_ptr) < 0) {
      ++i;
      swap_g(at_g(array,i,size), at_g(array,j,size), size);
    }

    ++j;
  }

  unsigned int l = umin((p-1-i), (end-p+1));
  array_block_swap_to_end_g(array, i+1, end, l, size);

  if(i==(unsigned int) -1) {
    *p1 = 0;
  } else {
    *p1 = i;
  }

  if(l == p-1-i) {
    *p2 = end-l+1;
  } else {
    *p2 = i+l+1;
  }
}


// int partition(const void** array, unsigned int start, unsigned int end, QSCompareFun compare) {
//   int pivot_pos = start + random_int(end-start);
//   const void* pivot = array[end];
//   swap( &array[end], &array[pivot_pos]);
//
//   int i = start;
//   for(int j=start; j <= end - 1; ++j) {
//     if(compare(array[j], pivot) < 0) {
//       swap(&array[i], &array[j]);
//       ++i;
//     }
//   }
//
//   swap(&array[i], &array[end]);
//   return i;
// }

static unsigned int partition(void** array, unsigned int first,unsigned int last, QSCompareFun compare){
  swap(&array[first], &array[first + random_int(last-first)]);
  const void* pivot = array[first];
  unsigned int i = first+1, j = last;
  while(i<=j){
    if(compare(array[i], pivot)<0)
      i++;
    else if(compare(array[j],pivot)>0)
      j--;
    else{
      swap(&array[i], &array[j]);
      i++;
      j--;
    }
  }
  swap(&array[first], &array[j]);
  return j;
}

void quick_sort_standard(void** array, unsigned int start, unsigned int end, QSCompareFun compare) {
  if( end <= start ) {
    return;
  }

  unsigned int pivot_pos = partition(array, start, end, compare);

  quick_sort_standard(array, start, pivot_pos, compare);
  quick_sort_standard(array, pivot_pos+1, end, compare );
}

void quick_sort_3_way(void** array, unsigned int start, unsigned int end, QSCompareFun compare) {
  if(end <= start ) {
    return;
  }

  unsigned int pivot_pos = start + random_int(end-start);
  unsigned int p1, p2;
  partition_3_way(array, start, end, pivot_pos, &p1, &p2, compare);

  quick_sort_3_way(array, start, p1, compare);
  quick_sort_3_way(array, p2, end, compare);
}

void quick_sort_3_way_g(void* array, unsigned int start, unsigned int end, unsigned int size, QSCompareFun compare) {
  if(end <= start ) {
    return;
  }

  unsigned int pivot_pos = start + random_int(end-start);
  unsigned int p1, p2;
  partition_3_way_g(array, start, end, pivot_pos, &p1, &p2, size, compare);

  quick_sort_3_way_g(array, start, p1, size, compare);
  quick_sort_3_way_g(array, p2, end, size, compare);
}


void quick_sort(void** array, unsigned int count, QSCompareFun compare) {
  if(count == 0) {
    return;
  }
  // quick_sort_standard(array, 0, count-1, compare);
  quick_sort_3_way(array, 0, count-1, compare);
}

void quick_sort_g(void* array, unsigned int count, unsigned int size, QSCompareFun fun) {
  if(count == 0) {
    return;
  }
  quick_sort_3_way_g(array, 0, count-1, size, fun);
}