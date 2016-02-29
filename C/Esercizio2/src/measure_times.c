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

PrintTime init_print_time(int argc, char const *argv[]) {
  KeyInfo keyInfo = KeyInfo_new(KeyInfo_string_compare, KeyInfo_string_hash);
  Dictionary header = Dictionary_new(keyInfo);
  Dictionary_set(header, "Esercizio", "1");
  Dictionary_set(header, "invocation", argv[0]);
  Dictionary_set(header, "field", argv[1]);

  PrintTime pt = PrintTime_new(header, NULL);

  Dictionary_free(header);
  KeyInfo_free(keyInfo);

  return pt;
}

int main(int argc, char const *argv[]) {
  check_arguments(argc, argv);

  PrintTime pt = init_print_time(argc, argv);

  __block Dataset* dataset;
  PrintTime_print(pt, "dataset_load", ^{
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


  PrintTime_print(pt, "Dictionary_load", ^{
    printf("Loading dictionary...\n");
    load_dictionary(dataset, dictionary);
    printf("Done!\n");
  });


  printf("Dictionary size: %d\n", Dictionary_size(dictionary));
  printf("Dictionary efficiency score: %f\n", Dictionary_efficiency_score(dictionary));

  PrintTime_print(pt, "Dictionary_iterate", ^{
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

  Record** records = Dataset_get_records(dataset);
  PrintTime_print(pt, "Dictionary_elem_access", ^{
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

  PrintTime_print(pt, "Dictionary_elem_deletion", ^{
    printf("Making 1_000_000 deletions\n");
    unsigned int size = Dataset_size(dataset);
    for(int i=0; i<1000000; ++i) {
      unsigned int index = drand48() * size;
      Record* record = records[index];
      Dictionary_delete(dictionary, record);
    }
    printf("Dictionary size: %d\n", Dictionary_size(dictionary));
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

  return 0;
}
