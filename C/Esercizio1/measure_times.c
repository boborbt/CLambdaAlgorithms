#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include "quick_sort.h"
#include "dataset.h"


// qsort passes to the comparison function a pointer to the array element,
// not the element itself. Dataset_compare_field? assumes that the given
// elements are actually the array elements themeselves. We need to dereferciate
// the pointers to make the things agree.
int qsort_compare_field1(const void* e1, const void* e2) {
  return Dataset_compare_field1( *(const void**)e1, *(const void**)e2 );
}

int qsort_compare_field2(const void* e1, const void* e2) {
  return Dataset_compare_field2( *(const void**)e1, *(const void**)e2 );
}

int qsort_compare_field3(const void* e1, const void* e2) {
  return Dataset_compare_field3( *(const void**)e1, *(const void**)e2 );
}



void time_quick_sort(Dataset* dataset, QSCompareFun compare) {
  clock_t start = clock();
  quick_sort((void**) Dataset_get_records(dataset), Dataset_get_size(dataset), compare);
  clock_t end = clock();

  printf("time: %10.2lf secs\n", ((double)end-start) / CLOCKS_PER_SEC);
}

void time_qsort(Dataset* dataset, QSCompareFun compare) {
  clock_t start = clock();
  qsort((void**) &Dataset_get_records(dataset)[0], Dataset_get_size(dataset), sizeof(Record*), compare);
  clock_t end = clock();

  printf("time: %10.2lf secs\n", ((double)end-start) / CLOCKS_PER_SEC);
}

void test_qsort(Dataset* dataset) {
  printf("Sorting according to field3\n");
  time_qsort(dataset, qsort_compare_field3);
  Dataset_print(dataset, 10);
  printf("Done!\n");

  printf("Sorting according to field2\n");
  time_qsort(dataset, qsort_compare_field2);
  Dataset_print(dataset, 10);
  printf("Done!\n");

  printf("Sorting according to field1\n");
  time_qsort(dataset, qsort_compare_field1);
  Dataset_print(dataset, 10);
  printf("Done!\n");
}

void test_quick_sort(Dataset* dataset) {
  printf("Sorting according to field3\n");
  time_quick_sort(dataset, Dataset_compare_field3);
  Dataset_print(dataset, 10);
  printf("Done!\n");

  printf("Sorting according to field2\n");
  time_quick_sort(dataset, Dataset_compare_field2);
  Dataset_print(dataset, 10);
  printf("Done!\n");

  printf("Sorting according to field1\n");
  time_quick_sort(dataset, Dataset_compare_field1);
  Dataset_print(dataset, 10);
  printf("Done!\n");
}

int main(int argc, char const *argv[]) {
  assert(argc > 1);

  printf("Loading dataset...\n");
  Dataset* dataset = Dataset_load(argv[1]);
  printf("Done!\n");

  test_quick_sort(dataset);

  Dataset_free(dataset);

  return 0;
}
