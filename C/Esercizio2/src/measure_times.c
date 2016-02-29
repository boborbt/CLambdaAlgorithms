#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dataset.h"
#include "dictionary.h"
#include "print_time.h"



void load_dictionary(Dataset* dataset, Dictionary dictionary) {
  Record** records = Dataset_get_records(dataset);
  unsigned int size = Dataset_size(dataset);

  for(int i=0; i < size; ++i) {
    if(i%1000000 == 0) {
      printf(".");
      fflush(stdout);
    }
    Dictionary_set(dictionary, records[i], records[i]);
  }

  printf("\n");
}

void add_time(FILE* file, char* label, double secs) {
  fprintf(file, "   %s: %lf\n", label, secs);
}

void print_usage() {
  printf("Usage: measure_time <field index> <file name>\n");
}

int char_included(char ch, char chars[], int size) {
  for(int i=0; i<size; ++i) {
    if(ch == chars[i]) return 1;
  }

  return 0;
}

void check_arguments(int argc, const char** argv) {
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

int main(int argc, char const *argv[]) {
  check_arguments(argc, argv);
  FILE* f_times = fopen("timings.txt", "a");
  assert(f_times);
  double secs;

  fprintf(f_times, "- invokation: %s\n", argv[0]);
  fprintf(f_times, "  field: %s\n", argv[1]);
  fprintf(f_times, "  data:\n");

  __block Dataset* dataset;
  secs = print_time(^{
    printf("Loading dataset...\n");
    dataset = Dataset_load(argv[2]);
    printf("Done!\n");
  });

  add_time(f_times, "ds_load", secs);

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
      exit(2);
  }

  dictionary = Dictionary_new(keyInfo);


  secs = print_time(^{
    printf("Loading dictionary...\n");
    load_dictionary(dataset, dictionary);
    printf("Done!\n");
  });

  add_time(f_times, "dic_load", secs);


  printf("Dictionary size: %d\n", Dictionary_size(dictionary));
  printf("Dictionary efficiency score: %f\n", Dictionary_efficiency_score(dictionary));

  secs = print_time(^{
    printf("Traversing the dictionary...\n");
    unsigned int count = 0;
    DictionaryIterator it = DictionaryIterator_new(dictionary);
    while(!DictionaryIterator_end(it)) {
      count += 1;
      DictionaryIterator_next(it);
    }
    printf("Counted %d elements\n", count);
    DictionaryIterator_free(it);
  });

  add_time(f_times, "iterating", secs);


  Record** records = Dataset_get_records(dataset);
  secs = print_time(^{
    printf("Making 1_000_000 accesses\n");
    unsigned int size = Dataset_size(dataset);
    for(int i=0; i<1000000; ++i) {
      unsigned int index = drand48() * size;
      Record* record = records[index];
      Record* result = NULL;
      if(!Dictionary_get(dictionary, record, (const void**)&result)) {
        printf("Cannot find record at index %d\n", index);
      };
    }
  });
  add_time(f_times, "elem_access", secs);

  secs = print_time(^{
    printf("Making 1_000_000 deletions\n");
    unsigned int size = Dataset_size(dataset);
    for(int i=0; i<1000000; ++i) {
      unsigned int index = drand48() * size;
      Record* record = records[index];
      Dictionary_delete(dictionary, record);
    }
    printf("Dictionary size: %d\n", Dictionary_size(dictionary));
  });
  add_time(f_times, "elem_del", secs);


  KeyInfo_free(keyInfo);

  secs = print_time(^{
    printf("Freeing dictionary\n");
    Dictionary_free(dictionary);
    printf("Done!\n");
  });
  add_time(f_times, "dic_dealloc", secs);


  print_time(^{
    printf("Freeing dataset\n");
    Dataset_free(dataset);
    printf("Done!\n");
  });
  add_time(f_times, "ds_dealloc", secs);


  return 0;
}
