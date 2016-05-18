#include "print_time.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/file.h>

#include "double_container.h"
#include "array.h"

struct _PrintTime {
  const char* file_name;
  FILE* file;
  Array header;
  Array data;
};

static void PrintTime_save_dictionary(PrintTime pt, const char* padding) {
  foreach_array_elem(pt->data, ^(void* elem) {
    KeyValue kv = *(KeyValue*)elem;
    fprintf(pt->file, "%s%s: %lf\n", padding, kv.key, DoubleContainer_get((DoubleContainer)kv.value));
  });
}

static void PrintTime_save_header(PrintTime pt) {
  fprintf(pt->file, "-\n");
  foreach_array_elem(pt->header, ^(void* elem) {
    KeyValue kv = *(KeyValue*)elem;
    fprintf(pt->file, "  %s: %s\n", kv.key, (const char*) kv.value);
  });

  fprintf(pt->file, "  data:\n");
}

static char* get_date_string() {
  static char buf[1024];
  time_t t;
  struct tm *tmp;
  t = time(NULL);
  tmp = localtime(&t);
  if(tmp == NULL) {
    perror("localtime failed");
    exit(1);
  }

  if(strftime(buf, 1024, "%a %b %d %H:%M:%S %Y", tmp) == 0) {
    fprintf(stderr, "Cannot format time\n");
    exit(1);
  }

  return buf;
}

PrintTime PrintTime_new(const char* out_file) {
  if(out_file==NULL) {
    out_file = "timings.yml";
  }

  PrintTime pt = (PrintTime) malloc(sizeof(struct _PrintTime));
  pt->file_name = out_file;
  pt->file = NULL;

  pt->header = Array_new(20, sizeof(KeyValue));
  pt->data = Array_new(20, sizeof(KeyValue));

  PrintTime_add_header(pt, "date", get_date_string());

  return pt;
}



void PrintTime_add_header(PrintTime pt, const char* key, const char* value) {
  KeyValue kv = { .key = strdup(key), .value = strdup(value) };
  Array_add(pt->header, &kv);
}

void PrintTime_free(PrintTime pt) {
  foreach_array_elem(pt->header, ^(void* elem) {
    KeyValue kv = *(KeyValue*) elem;
    free(kv.key);
    free(kv.value);
  });
  Array_free(pt->header);

  foreach_array_elem(pt->data, ^(void* elem) {
    KeyValue kv = *(KeyValue*) elem;
    free(kv.key);
    DoubleContainer_free((DoubleContainer) kv.value);
  });
  Array_free(pt->data);

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

 KeyValue kv = { .key = strdup(label), .value = DoubleContainer_new(result) };
 Array_add(pt->data, &kv);

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
