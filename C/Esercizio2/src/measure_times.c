#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dataset.h"
#include "dictionary.h"
#include "print_time.h"
#include "errors.h"
#include "macros.h"
#include "iterator_functions.h"
#include "mem.h"

static void load_dictionary(Dataset* dataset, Dictionary dictionary) {
  Record** records = Dataset_get_records(dataset);
  size_t size = Dataset_size(dataset);

  for(size_t i=0; i < size; ++i) {
    if(i%1000000 == 0) {
      printf(".");
      fflush(stdout);
    }
    Dictionary_set(dictionary, records[i], records[i]);
  }

  printf("\n");
}

static void print_usage() {
  printf("Usage: measure_time <field index> <file name>\n");
}

static int char_included(char ch, char chars[], size_t size) {
  for(size_t i=0; i<size; ++i) {
    if(ch == chars[i]) return 1;
  }

  return 0;
}

static void check_arguments(int argc, char** argv) {
  if(argc < 3) {
    print_usage();
    exit(1);
  }

  if(!char_included(argv[1][0], (char[]){'1','2','3'}, 3)) {
    printf("field index must be one of {1, 2, 3}\n");
    print_usage();
    exit(1);
  }
}

static char* get_compilation_flags() {
  static char buf[4096];
  FILE* file = fopen("Makefile.vars", "r");
  if(file==NULL) {
    Error_raise(Error_new(ERROR_FILE_READING, "Cannot open Makefile.vars to read compilatin flags" ));
  }

  int found = 0;
  while( !found && fgets(buf, 4096, file) != NULL) {
    found =  strstr(buf, "CFLAGS") != NULL;
  }

  if(!found) {
    Error_raise(Error_new(ERROR_FILE_READING, "Cannot find CFLAGS string into Makefile.vars"));
  }

  buf[strlen(buf)-1] = '\0'; // removing trailing \n
  return buf;
}

static PrintTime init_print_time(char* argv[]) {
  PrintTime pt = PrintTime_new(NULL);

  PrintTime_add_header(pt,"esercizio", "2");
  PrintTime_add_header(pt, "field", argv[1]);
  PrintTime_add_header(pt, "invocation", argv[0]);
  PrintTime_add_header(pt, "compilation_flags", get_compilation_flags());

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


  Dictionary dictionary;
  KeyInfo keyInfo;
  switch(argv[1][0]) {
    case '1':
      keyInfo = KeyInfo_new(Dataset_compare_field1, Dataset_hash_field1);
      break;
    case '2':
      keyInfo = KeyInfo_new(Dataset_compare_field2, Dataset_hash_field2);
      break;
    case '3':
      keyInfo = KeyInfo_new(Dataset_compare_field3, Dataset_hash_field3);
      break;
    default:
      Error_raise(Error_new(ERROR_ARGUMENT_PARSING, "Index field was expected to be in {1,2,3} but was %d", argv[1][0]));
  }

  dictionary = Dictionary_new(keyInfo);


  PrintTime_print(pt, "Dictionary_load", ^{
    printf("Loading dictionary...\n");
    load_dictionary(dataset, dictionary);
    printf("Done!\n");
  });


  printf("Dictionary size: %ld\n", Dictionary_size(dictionary));
  printf("Dictionary efficiency score: %f\n", Dictionary_efficiency_score(dictionary));

  PrintTime_print(pt, "Dictionary_iterate", ^{
    printf("Traversing the dictionary...\n");
    __block size_t count = 0;
    for_each(Dictionary_it(dictionary),  ^(UNUSED(void* kv)) {
      count += 1;
    });
    printf("Counted %ld elements\n", count);
  });

  Record** records = Dataset_get_records(dataset);
  PrintTime_print(pt, "Dictionary_elem_access", ^{
    printf("Making 1_000_000 accesses\n");
    size_t size = Dataset_size(dataset);
    for(int i=0; i<1000000; ++i) {
      size_t index = (size_t)(drand48() * size);
      Record* record = records[index];
      Record* result = NULL;
      if(!Dictionary_get(dictionary, record, (void**)&result)) {
        printf("Cannot find record at index %ld\n", index);
      };
    }
  });

  PrintTime_print(pt, "Dictionary_elem_deletion", ^{
    printf("Making 1_000_000 deletions\n");
    assert(Dictionary_check_integrity(dictionary));

    size_t size = Dataset_size(dataset);
    for(int i=0; i<1000000; ++i) {
      size_t index = (size_t)(drand48() * size);
      Record* record = records[index];

      Dictionary_delete(dictionary, record);
    }

    assert(Dictionary_check_integrity(dictionary));
    printf("Dictionary size: %ld\n", Dictionary_size(dictionary));
  });

  KeyInfo_free(keyInfo);

  PrintTime_print(pt, "Dictionary_free", ^{
    printf("Freeing dictionary\n");
    Dictionary_free(dictionary);
    printf("Done!\n");
  });


  PrintTime_print(pt, "Dataset_free", ^{
    printf("Freeing dataset\n");
    Dataset_free(dataset);
    printf("Done!\n");
  });

  PrintTime_save(pt);
  PrintTime_free(pt);

  Mem_check_and_report();

  return 0;
}
