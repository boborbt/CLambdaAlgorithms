#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "quick_sort.h"
#include "merge_sort.h"
#include "heap_sort.h"
#include "dataset.h"
#include "print_time.h"


// qsort passes to the comparison function a pointer to the array element,
// not the element itself. Dataset_compare_field? assumes that the given
// elements are actually the array elements themeselves. We need to dereference
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


void test_qsort(Dataset* dataset) {
  print_time(^{
    printf("Sorting according to field1\n");
    qsort((void**) Dataset_get_records(dataset), Dataset_size(dataset), sizeof(Record*), qsort_compare_field1);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);

  print_time(^{
    printf("Sorting according to field2\n");
    qsort((void**) Dataset_get_records(dataset), Dataset_size(dataset), sizeof(Record*), qsort_compare_field2);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);

  print_time(^{
    printf("Sorting according to field3\n");
    qsort((void**) Dataset_get_records(dataset), Dataset_size(dataset), sizeof(Record*), qsort_compare_field3);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);
}


void test_algorithm(Dataset* dataset, void (*sort)(const void**, int, int(*)(const void*, const void*))) {
  print_time(^{
    printf("Sorting according to field1\n");
    sort((const void**)Dataset_get_records(dataset), Dataset_size(dataset), Dataset_compare_field1);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);

  print_time(^{
    printf("Sorting according to field2\n");
    sort((const void**)Dataset_get_records(dataset), Dataset_size(dataset), Dataset_compare_field2);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);

  print_time(^{
    printf("Sorting according to field3\n");
    sort((const void**)Dataset_get_records(dataset), Dataset_size(dataset), Dataset_compare_field3);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);
}


void print_usage() {
  printf("Usage: measure_time <opt> <file name>\n");
  printf(" opts: -q use quick_sort algorithm\n");
  printf("       -Q use system qsort algorithm\n");
  printf("       -m use merge_sort algorithm\n");
  printf("       -h use heap_sort algorithm\n");
  printf("       -h print this message\n");
}

int char_included(char ch, char chars[], int size) {
  for(int i=0; i<size; ++i) {
    if(ch == chars[i]) return 1;
  }

  return 0;
}

void check_arguments(int argc, const char** argv) {
  if(argc <= 2) {
    print_usage();
    exit(1);
  }

  if(strlen(argv[1])!=2 || argv[1][0] != '-' || !char_included(argv[1][1], (char[]){'q','m','h','Q', 'H'}, 5)) {
    printf("Option %s not recognized\n", argv[1]);
    print_usage();
    exit(1);
  }

  if(!strcmp(argv[1], "-h")) {
    print_usage();
    exit(1);
  }
}

int main(int argc, char const *argv[]) {
  check_arguments(argc, argv);

  __block Dataset* dataset;
  print_time(^{
    printf("Loading dataset...\n");
    dataset = Dataset_load(argv[2]);
    printf("Done!\n");
  });

  Dataset_print(dataset, 10);

  switch(argv[1][1]) {
    case 'q':
      test_algorithm(dataset, quick_sort);
      break;
    case 'm':
      test_algorithm(dataset, merge_sort);
      break;
    case 'H':
      test_algorithm(dataset, heap_sort);
      break;
    case 'Q':
      test_qsort(dataset);
      break;
    default:
      assert(0); // should never get here
  }

  print_time(^{
    printf("Freeing dataset\n");
    Dataset_free(dataset);
    printf("Done!\n");
  });

  return 0;
}
