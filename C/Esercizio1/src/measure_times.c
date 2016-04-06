#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "quick_sort.h"
#include "insertion_sort.h"
#include "merge_sort.h"
#include "heap_sort.h"
#include "dataset.h"
#include "print_time.h"


// qsort passes to the comparison function a pointer to the array element,
// not the element itself. Dataset_compare_field? assumes that the given
// elements are actually the array elements themeselves. We need to dereference
// the pointers to make the things agree.
static int qsort_compare_field1(const void* e1, const void* e2) {
  return Dataset_compare_field1( *( void* const*)e1, *(void* const*)e2 );
}

static int qsort_compare_field2(const void* e1, const void* e2) {
  return Dataset_compare_field2( *(void* const*)e1, *(void* const*)e2 );
}

static int qsort_compare_field3(const void* e1, const void* e2) {
  return Dataset_compare_field3( *(void* const*)e1, *(void* const*)e2 );
}


static void test_qsort(Dataset* dataset, PrintTime pt) {
  PrintTime_print(pt, "field1", ^{
    printf("Sorting according to field1\n");
    qsort((void**) Dataset_get_records(dataset), Dataset_size(dataset), sizeof(Record*), qsort_compare_field1);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);

  PrintTime_print(pt, "field2", ^{
    printf("Sorting according to field2\n");
    qsort((void**) Dataset_get_records(dataset), Dataset_size(dataset), sizeof(Record*), qsort_compare_field2);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);

  PrintTime_print(pt, "field3", ^{
    printf("Sorting according to field3\n");
    qsort((void**) Dataset_get_records(dataset), Dataset_size(dataset), sizeof(Record*), qsort_compare_field3);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);
}


static void test_algorithm_g(Dataset* dataset, PrintTime pt, void (*sort)(void*, unsigned int, unsigned int, int(*)(const void*, const void*))) {
  PrintTime_print(pt, "field1", ^{
    printf("Sorting according to field1\n");
    sort((void**)Dataset_get_records(dataset), Dataset_size(dataset), sizeof(void*), Dataset_compare_field1_g);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);

  PrintTime_print(pt, "field2", ^{
    printf("Sorting according to field2\n");
    sort((void**)Dataset_get_records(dataset), Dataset_size(dataset), sizeof(void*), Dataset_compare_field2_g);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);

  PrintTime_print(pt, "field3", ^{
    printf("Sorting according to field3\n");
    sort((void**)Dataset_get_records(dataset), Dataset_size(dataset), sizeof(void*), Dataset_compare_field3_g);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);
}

static void test_algorithm(Dataset* dataset, PrintTime pt, void (*sort)(void**, unsigned int, int(*)(const void*, const void*))) {
  PrintTime_print(pt, "field1", ^{
    printf("Sorting according to field1\n");
    sort((void**)Dataset_get_records(dataset), Dataset_size(dataset), Dataset_compare_field1);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);

  PrintTime_print(pt, "field2", ^{
    printf("Sorting according to field2\n");
    sort((void**)Dataset_get_records(dataset), Dataset_size(dataset), Dataset_compare_field2);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);

  PrintTime_print(pt, "field3", ^{
    printf("Sorting according to field3\n");
    sort((void**)Dataset_get_records(dataset), Dataset_size(dataset), Dataset_compare_field3);
    printf("Done!\n");
  });
  Dataset_print(dataset, 10);
}


static void print_usage() {
  printf("Usage: measure_time <opt> <file name>\n");
  printf(" opts: -q use quick_sort algorithm\n");
  printf("       -Q use quick_sort_g algorithm\n");
  printf("       -i use insertion_sort algorithm\n");
  printf("       -s use system qsort algorithm\n");
  printf("       -m use merge_sort algorithm\n");
  printf("       -M use merge_sort_g algorithm\n");
  printf("       -H use heap_sort algorithm\n");
  printf("       -h print this message\n");
}

static int char_included(char ch, char chars[], unsigned int size) {
  for(unsigned int i=0; i<size; ++i) {
    if(ch == chars[i]) return 1;
  }

  return 0;
}

static void check_arguments(int argc, char** argv) {
  if(argc <= 2) {
    print_usage();
    exit(1);
  }

  if(strlen(argv[1])!=2 || argv[1][0] != '-' || !char_included(argv[1][1], (char[]){'q','Q','m','M','h','s', 'H', 'i'}, 8)) {
    printf("Option %s not recognized\n", argv[1]);
    print_usage();
    exit(1);
  }

  if(!strcmp(argv[1], "-h")) {
    print_usage();
    exit(1);
  }
}

static char* flag_to_algorithm_name(char ch) {
  switch(ch) {
    case 'i': return "insertion_sort";
    case 'q': return "quick_sort";
    case 'Q': return "quick_sort_g";
    case 's': return "system_quick_sort";
    case 'M': return "merge_sort_g";
    case 'm': return "merge_sort";
    case 'H': return "heap_sort";
    default:
      printf("Flag not in the known set");
      exit(1);
  };
}

static PrintTime init_print_time(char* argv[]) {
  KeyInfo keyInfo = KeyInfo_new(KeyInfo_string_compare, KeyInfo_string_hash);
  Dictionary header = Dictionary_new(keyInfo);
  Dictionary_set(header, "Esercizio", "1");
  Dictionary_set(header, "invocation", argv[0]);
  Dictionary_set(header, "algorithm", flag_to_algorithm_name(argv[1][1]));

  PrintTime pt = PrintTime_new(header, NULL);

  Dictionary_free(header);
  KeyInfo_free(keyInfo);

  return pt;
}


int main(int argc, char* argv[]) {
  check_arguments(argc, argv);
  PrintTime pt = init_print_time(argv);


  __block Dataset* dataset;
  PrintTime_print(pt, "Dataset_load", ^{
    printf("Loading dataset...\n");
    dataset = Dataset_load(argv[2]);
    printf("Done!\n");
  });

  Dataset_print(dataset, 10);

  switch(argv[1][1]) {
    case 'q':
      test_algorithm(dataset, pt, quick_sort);
      break;
    case 'Q':
      test_algorithm_g(dataset, pt, quick_sort_g);
      break;
    case 'i':
      test_algorithm(dataset, pt, insertion_sort);
      break;
    case 'm':
      test_algorithm(dataset, pt, merge_sort);
      break;
    case 'M':
      test_algorithm_g(dataset, pt, merge_sort_g);
      break;
    case 'H':
      test_algorithm(dataset, pt, heap_sort);
      break;
    case 's':
      test_qsort(dataset, pt);
      break;
    default:
      assert(0); // should never get here
  }

  PrintTime_print(pt, "Dataset_free", ^{
    printf("Freeing dataset\n");
    Dataset_free(dataset);
    printf("Done!\n");
  });

  PrintTime_save(pt);
  PrintTime_free(pt);

  return 0;
}
