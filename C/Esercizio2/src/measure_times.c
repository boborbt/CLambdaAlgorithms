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
    Dictionary_set(dictionary, records[i], records[i]);
  }
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
  switch(argv[1][0]) {
    case '1':
      dictionary = Dictionary_new(Dataset_compare_field1);
      break;
    case '2':
      dictionary = Dictionary_new(Dataset_compare_field2);
      break;
    case '3':
      dictionary = Dictionary_new(Dataset_compare_field3);
      break;
    default:
      exit(2);
  }

  print_time(^{
    printf("Loading dictionary...\n");
    load_dictionary(dataset, dictionary);
    printf("Done!\n");
  });

  printf("Dictionary size: %d\n", Dictionary_size(dictionary));
  printf("Search tree height: %d\n", Dictionary_efficiency_score(dictionary));


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
