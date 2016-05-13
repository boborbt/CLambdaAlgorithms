#include <assert.h>
#include <stdlib.h>
#include "quick_sort.h"
#include "merge_sort.h"
#include "heap_sort.h"
#include "insertion_sort.h"
#include "unit_testing.h"
#include "array_g.h"
#include <stdio.h>

void partition_3_way(void** array, size_t start, size_t end, size_t pivot_pos, size_t* p1, size_t* p2,  QSCompareFun compare);

static int compare(const void* elem1, const void* elem2) {
  long int e1 = (long int) elem1;
  long int e2 = (long int) elem2;

  if(e1 < e2) return -1;
  if(e1 > e2) return 1;

  return 0;
}

static int compare_g(const void* elem1, const void* elem2) {
  int e1 = *(const int*) elem1;
  int e2 = *(const int*) elem2;
  return e1 - e2;
}

static void test_quick_sort_empty_array() {
  long a[1];
  quick_sort((void**)a, 0, compare);

  assert(TRUE); // suffices that the algorithm terminates
}

static void test_merge_sort_empty_array() {
  long a[1];
  merge_sort((void**)a, 0, compare);

  assert(TRUE); // suffices that the algorithm terminates
}

static void test_heap_sort_empty_array() {
  long a[1];
  heap_sort((void**)a, 0, compare);

  assert(TRUE); // suffices that the algorithm terminates
}

static void test_insertion_sort_empty_array() {
  long a[1];
  insertion_sort((void**)a, 0, compare);

  assert(TRUE); // suffices that the algorithm terminates
}


static void test_quick_sort_null_array() {
  quick_sort((void**) NULL, 0, compare);

  assert(TRUE); // suffices that the algorithm terminates
}

static void test_merge_sort_null_array() {
  merge_sort((void**) NULL, 0, compare);

  assert(TRUE); // suffices that the algorithm terminates
}

static void test_heap_sort_null_array() {
  heap_sort((void**) NULL, 0, compare);

  assert(TRUE); // suffices that the algorithm terminates
}

static void test_insertion_sort_null_array() {
  insertion_sort((void**) NULL, 0, compare);

  assert(TRUE); // suffices that the algorithm terminates
}

static void test_quick_sort_single_element_array() {
  long a[] = { 100 };
  quick_sort((void**)a, 1, compare);

  assert( a[0] == 100 );
}

static void test_merge_sort_single_element_array() {
  long a[] = { 100 };
  merge_sort((void**)a, 1, compare);

  assert( a[0] == 100 );
}

static void test_heap_sort_single_element_array() {
  long a[] = { 100 };
  heap_sort((void**)a, 1, compare);

  assert( a[0] == 100 );
}

static void test_insertion_sort_single_element_array() {
  long a[] = { 100 };
  insertion_sort((void**)a, 1, compare);

  assert( a[0] == 100 );
}


static void test_quick_sort_all_repetitions_array() {
  long a[] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
  quick_sort((void**) a, 14, compare);

  for(int i=0; i < 14; ++i) {
    assert( a[i] == 10 );
  }
}

static void test_quick_sort_full_array() {
  long a[] = { 85, 91, 49, 16, 31, 26, 96, 83, 60, 80 };
  quick_sort((void**) a, 10, compare);

  for(int i=0; i<9; ++i) {
    assert(a[i] <= a[i+1]);
  }
}

static void test_quick_sort_g_full_array() {
  int a[] = { 85, 91, 49, 16, 31, 26, 96, 83, 60, 80 };
  quick_sort_g((void*) a, 10, sizeof(int), compare_g);

  for(int i=0; i<9; ++i) {
    assert(a[i] <= a[i+1]);
  }
}

static void test_merge_sort_full_array() {
  long a[] = { 85, 91, 49, 16, 31, 26, 96, 83, 60, 80 };
  merge_sort((void**) a, 10, compare);

  for(int i=0; i<9; ++i) {
    assert(a[i] <= a[i+1]);
  }
}

static void test_heap_sort_full_array() {
  long a[] = { 85, 91, 49, 16, 31, 26, 96, 83, 60, 80 };
  heap_sort((void**) a, 10, compare);

  for(int i=0; i<9; ++i) {
    assert(a[i] <= a[i+1]);
  }
}

static void test_insertion_sort_full_array() {
  long a[] = { 85, 91, 49, 16, 31, 26, 96, 85, 60, 80 };
  insertion_sort((void**) a, 10, compare);

  for(int i=0; i<9; ++i) {
    assert(a[i] <= a[i+1]);
  }
}

static void test_partition_3_way_case1() {
  long int a[14] = { 1, 10, 3, 4, 10, 2, 10, 25, 22, 10, 11, 10, 9, 10 };
  long int expected[14] = { 1, 9, 3, 4, 2, 10, 10, 10, 10, 10, 10, 11, 22, 25 };

  size_t p1;
  size_t p2;
  partition_3_way((void**)a, 0, 13, 1, &p1, &p2, compare);
  assert_equal( (long)4, (long)p1);
  assert_equal( (long)11, (long)p2);
  for(int i=0; i<14; ++i) {
    assert_equal(a[i], expected[i]);
  }
}

static void test_partition_3_way_case2() {
  long int a[14] = { 1, 10, 3, 4, 12, 2, 15, 25, 22, 10, 11, 11, 9, 10 };
  long int expected[14] = { 1, 9, 3, 4, 2, 10, 10, 10, 22, 11, 11, 12, 15, 25 };

  size_t p1;
  size_t p2;
  partition_3_way((void**)a, 0, 13, 1, &p1, &p2, compare);
  assert_equal( (long)4, (long)p1);
  assert_equal( (long)8, (long)p2);
  for(int i=0; i<14; ++i) {
    assert_equal(a[i], expected[i]);
  }
}

static void test_partition_3_way_1_pivot() {
  long int a[14] = { 1, 10, 3, 4, 12, 2, 15, 25, 22, 14, 11, 11, 9, 9 };
  long int expected[14] = { 1, 9, 3, 4, 2, 9, 10, 25, 22, 14, 11, 11, 12, 15 };

  size_t p1;
  size_t p2;
  partition_3_way((void**)a, 0, 13, 1, &p1, &p2, compare);
  assert_equal( (long)5, (long)p1);
  assert_equal( (long)7, (long)p2);
  for(size_t i=0; i<14; ++i) {
    assert_equal(a[i], expected[i]);
  }
}

static void test_partition_3_way_all_pivots() {
  long int a[14] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };
  long int expected[14] = { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 };

  size_t p1;
  size_t p2;
  partition_3_way((void**)a, 0, 13, 1, &p1, &p2, compare);
  assert_equal( (long)0, (long)p1);
  assert_equal( (long)14, (long)p2);
  for(int i=0; i<14; ++i) {
    assert_equal(a[i], expected[i]);
  }
}

static void test_array_g_swap_at() {
  int array[] = { 12, 11, 2, 1, 21, 15, 18 };
  swap_g(at_g(array, 1, sizeof(int)), at_g(array,4,sizeof(int)), sizeof(int));
  assert_equal32(array[1], 21);
  assert_equal32(array[4], 11);
}

static void test_array_g_swap() {
  int array[] = { 12, 11, 2, 1, 21, 15, 18 };
  swap_g(&array[1], &array[4], sizeof(int));
  assert_equal32(array[1], 21);
  assert_equal32(array[4], 11);
}

static void test_array_g_at() {
  int array[] = { 12, 11, 2, 1, 21, 15, 18 };
  assert_equal32(*(int*)at_g(array, 2, sizeof(int)), 2);
}

static void test_array_g_cp() {
  int array[] = { 12, 11, 2, 1, 21, 15, 18 };
  cp_g(&array[2], &array[5], sizeof(int));
  assert_equal32(array[2], 15);
}


int main() {
  start_tests("partition");
  test(test_partition_3_way_case1);
  test(test_partition_3_way_case2);
  test(test_partition_3_way_1_pivot);
  test(test_partition_3_way_all_pivots);
  end_tests();

  start_tests("quick_sort");
  test(test_quick_sort_empty_array);
  test(test_quick_sort_null_array);
  test(test_quick_sort_single_element_array);
  test(test_quick_sort_all_repetitions_array);
  test(test_quick_sort_full_array);
  end_tests();

  start_tests("merge_sort");
  test(test_merge_sort_empty_array);
  test(test_merge_sort_null_array);
  test(test_merge_sort_single_element_array);
  test(test_merge_sort_full_array);
  end_tests();

  start_tests("heap_sort");
  test(test_heap_sort_empty_array);
  test(test_heap_sort_null_array);
  test(test_heap_sort_single_element_array);
  test(test_heap_sort_full_array);
  end_tests();

  start_tests("insertion_sort");
  test(test_insertion_sort_empty_array);
  test(test_insertion_sort_null_array);
  test(test_insertion_sort_single_element_array);
  test(test_insertion_sort_full_array);
  end_tests();

  start_tests("quick_sort_g");
  test(test_quick_sort_g_full_array);
  end_tests();

  start_tests("array_g");
  test(test_array_g_swap_at);
  test(test_array_g_swap);
  test(test_array_g_at);
  test(test_array_g_cp);
  end_tests();

  return 0;
}
