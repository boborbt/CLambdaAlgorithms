#include "print_time.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "double_container.h"

struct _PrintTime {
  FILE* file;
  Dictionary data;
};

static void PrintTime_save_dictionary(PrintTime pt, Dictionary dic, const char* padding) {
  DictionaryIterator it = DictionaryIterator_new(dic);

  while(!DictionaryIterator_end(it)) {
    Elem elem = DictionaryIterator_get(it);
    fprintf(pt->file, "%s%s: %lf\n", padding, elem->key, DoubleContainer_get((DoubleContainer)elem->value));
    DictionaryIterator_next(it);
  }

  DictionaryIterator_free(it);
}

static void PrintTime_save_header(PrintTime pt, Dictionary header) {
  fprintf(pt->file, "-\n");
  DictionaryIterator it = DictionaryIterator_new(header);

  while(!DictionaryIterator_end(it)) {
    Elem elem = DictionaryIterator_get(it);
    fprintf(pt->file, "  %s: %s\n", elem->key, elem->value);
    DictionaryIterator_next(it);
  }

  DictionaryIterator_free(it);

  fprintf(pt->file, "  data:\n");
}

PrintTime PrintTime_new(Dictionary header, const char* out_file) {
  if(out_file==NULL) {
    out_file = "timings.yml";
  }
  PrintTime pt = (PrintTime) malloc(sizeof(struct _PrintTime));
  pt->file = fopen(out_file, "a");
  if(!pt->file) {
    perror("Cannot open output file");
    exit(1);
  }

  KeyInfo keyInfo = KeyInfo_new(KeyInfo_string_compare, KeyInfo_string_hash);
  pt->data = Dictionary_new(keyInfo);

  PrintTime_save_header(pt, header);

  return pt;
}

void PrintTime_free(PrintTime pt) {
  DictionaryIterator it = DictionaryIterator_new(pt->data);
  while(!DictionaryIterator_end(it)) {
    DoubleContainer_free((DoubleContainer) DictionaryIterator_get(it)->value);
    DictionaryIterator_next(it);
  }

  DictionaryIterator_free(it);
  Dictionary_free(pt->data);
  fclose(pt->file);
  free(pt);
}


double PrintTime_print(PrintTime pt, char* label, void(^fun)()) {
 double result;
 printf("\n======================\n");
 clock_t start = clock();
 fun();
 clock_t end = clock();

 result = ((double)end-start) / CLOCKS_PER_SEC;

 printf("----------------------\n");
 printf("time:  %10.2lf secs\n", result);
 printf("======================\n\n");

 Dictionary_set(pt->data, label, DoubleContainer_new(result));

 return result;
}

void PrintTime_save(PrintTime pt) {
  PrintTime_save_dictionary(pt, pt->data, "    ");
}
