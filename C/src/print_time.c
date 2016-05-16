#include "print_time.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/file.h>

#include "double_container.h"

struct _PrintTime {
  const char* file_name;
  FILE* file;
  Dictionary header;
  Dictionary data;
};

static void PrintTime_save_dictionary(PrintTime pt, const char* padding) {
  foreach_dictionary_elem(pt->data, ^(Elem* elem) {
    fprintf(pt->file, "%s%s: %lf\n", padding, elem->key, DoubleContainer_get((DoubleContainer)elem->value));
  });
}

static void PrintTime_save_header(PrintTime pt) {
  fprintf(pt->file, "-\n");
  foreach_dictionary_elem(pt->header, ^(Elem* elem) {
    fprintf(pt->file, "  %s: %s\n", elem->key, elem->value);
  });

  fprintf(pt->file, "  data:\n");
}

PrintTime PrintTime_new(const char* out_file) {
  if(out_file==NULL) {
    out_file = "timings.yml";
  }

  PrintTime pt = (PrintTime) malloc(sizeof(struct _PrintTime));
  pt->file_name = out_file;
  pt->file = NULL;

  KeyInfo keyInfo = KeyInfo_new(KeyInfo_string_compare, KeyInfo_string_hash);
  pt->header = Dictionary_new(keyInfo);
  pt->data = Dictionary_new(keyInfo);

  return pt;
}

void PrintTime_add_header(PrintTime pt, const char* key, const char* value) {
  Dictionary_set(pt->header, strdup(key), strdup(value));
}

void PrintTime_free(PrintTime pt) {
  foreach_dictionary_elem(pt->header, ^(Elem* elem) {
    free(elem->key);
    free(elem->value);
  });
  Dictionary_free(pt->header);

  foreach_dictionary_elem(pt->data, ^(Elem* elem) {
    DoubleContainer_free((DoubleContainer) elem->value);
  });
  Dictionary_free(pt->data);

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

  pt->file = fopen(pt->file_name, "a");
  if(!pt->file) {
    perror("Cannot open output file");
    exit(1);
  }

  if(flock(fileno(pt->file), LOCK_EX) != 0) {
    perror("Cannot lock output file");
    fclose(pt->file);
    exit(1);
  }

  PrintTime_save_header(pt);
  PrintTime_save_dictionary(pt, "    ");

  if(flock(fileno(pt->file), LOCK_UN) != 0) {
    perror("Cannot unlock output file");
    fclose(pt->file);
    exit(1);
  }

  fclose(pt->file);
}
