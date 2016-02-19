#include <assert.h>
#include <stdlib.h>
#include "quick_sort.h"
#include <stdio.h>

void partition_3_way(void** array, int start, int end, int pivot_pos, int* p1, int* p2,  QSCompareFun compare);

#define TRUE 1
#define FALSE 0

#define assert_equal(a,b) if((a)!=(b)) {\
   printf("assertion failed at file:%s line:%d. %ld was expected to be equal to %ld\n",\
    __FILE__, __LINE__, (a), (b)); exit(1); }


int compare(const void* elem1, const void* elem2) {
  int e1 = (long) elem1;
  int e2 = (long) elem2;

  if(e1 < e2) return -1;
  if(e1 > e2) return 1;

  return 0;
}

void test_quick_sort_empty_array() {
  long a[] = {};
  quick_sort((void**)a, 0, compare);

  assert(TRUE); // suffices that the algorithm terminates
}

void test_quick_sort_null_array() {
  quick_sort((void**) NULL, 0, compare);

  assert(TRUE); // suffices that the algorithm terminates
}

void test_quick_sort_single_element_array() {
  long a[] = { 100 };
  quick_sort((void**)a, 1, compare);

  assert( a[0] == 100 );
}

void test_quick_sort_all_repetitions_array() {
  long a[] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
  quick_sort((void**) a, 14, compare);

  for(int i=0; i < 14; ++i) {
    assert( a[i] == 10 );
  }
}

void test_quick_sort_full_array() {
  long a[] = { 85, 91, 49, 16, 31, 26, 96, 83, 60, 80 };
  quick_sort((void**) a, 10, compare);

  for(int i=0; i<9; ++i) {
    assert(a[i] <= a[i+1]);
  }
}

void test_partition_3_way_case1() {
  long int a[14] = { 1, 10, 3, 4, 10, 2, 10, 25, 22, 10, 11, 10, 9, 10 };
  long int expected[14] = { 1, 9, 3, 4, 2, 10, 10, 10, 10, 10, 10, 11, 22, 25 };

  int p1;
  int p2;
  partition_3_way((void**)a, 0, 13, 1, &p1, &p2, compare);
  assert_equal( (long)4, (long)p1);
  assert_equal( (long)11, (long)p2);
  for(int i=0; i<14; ++i) {
    assert_equal(a[i], expected[i]);
  }
}

void test_partition_3_way_case2() {
  long int a[14] = { 1, 10, 3, 4, 12, 2, 15, 25, 22, 10, 11, 11, 9, 10 };
  long int expected[14] = { 1, 9, 3, 4, 2, 10, 10, 10, 22, 11, 11, 12, 15, 25 };

  int p1;
  int p2;
  partition_3_way((void**)a, 0, 13, 1, &p1, &p2, compare);
  assert_equal( (long)4, (long)p1);
  assert_equal( (long)8, (long)p2);
  for(int i=0; i<14; ++i) {
    assert_equal(a[i], expected[i]);
  }
}

void test_partition_3_way_1_pivot() {
  long int a[14] = { 1, 10, 3, 4, 12, 2, 15, 25, 22, 14, 11, 11, 9, 9 };
  long int expected[14] = { 1, 9, 3, 4, 2, 9, 10, 25, 22, 14, 11, 11, 12, 15 };

  int p1;
  int p2;
  partition_3_way((void**)a, 0, 13, 1, &p1, &p2, compare);
  assert_equal( (long)5, (long)p1);
  assert_equal( (long)7, (long)p2);
  for(int i=0; i<14; ++i) {
    assert_equal(a[i], expected[i]);
  }
}

void test_partition_3_way_all_pivots() {
  long int a[14] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
  long int expected[14] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };

  int p1;
  int p2;
  partition_3_way((void**)a, 0, 13, 1, &p1, &p2, compare);
  assert_equal( (long)-1, (long)p1);
  assert_equal( (long)14, (long)p2);
  for(int i=0; i<14; ++i) {
    assert_equal(a[i], expected[i]);
  }
}



int main(int argc, char const *argv[]) {
  test_partition_3_way_case1();
  test_partition_3_way_case2();
  test_partition_3_way_1_pivot();
  test_partition_3_way_all_pivots();


  test_quick_sort_empty_array();
  test_quick_sort_null_array();
  test_quick_sort_single_element_array();
  test_quick_sort_all_repetitions_array();
  test_quick_sort_full_array();

  return 0;
}
