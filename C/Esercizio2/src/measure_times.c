#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dataset.h"
#include "dictionary.h"


void print_time(void(^fun)()) {
  clock_t start = clock();
  fun();
  clock_t end = clock();

  printf("time: %10.2lf secs\n", ((double)end-start) / CLOCKS_PER_SEC);
}


void load_dictionary(Dataset* dataset, Dictionary dictionary) {
  Record** records = Dataset_get_records(dataset);
  unsigned int size = Dataset_get_size(dataset);

  for(int i=0; i < size; ++i) {
    if(i%1000000 == 0) {
      printf(".");
      fflush(stdout);
    }
    Dictionary_set(dictionary, records[i], records[i]);
  }

  printf("\n");
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

  __block Dataset* dataset;
  print_time(^{
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
      exit(2);
  }

  dictionary = Dictionary_new(keyInfo);


  print_time(^{
    printf("Loading dictionary...\n");
    load_dictionary(dataset, dictionary);
    printf("Done!\n");
  });

  printf("Dictionary size: %d\n", Dictionary_size(dictionary));
  printf("Dictionary efficiency score: %f\n", Dictionary_efficiency_score(dictionary));

  print_time(^{
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

  free(keyInfo);


  print_time(^{
    printf("Freeing dictionary\n");
    Dictionary_free(dictionary);
    printf("Done!\n");
  });

  print_time(^{
    printf("Freeing dataset\n");
    Dataset_free(dataset);
    printf("Done!\n");
  });


  return 0;
}
