#include "print_time.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/file.h>
#include <errno.h>
#include <string.h>

#include "double_container.h"
#include "array_alt.h"
#include "iterator_functions.h"
#include "errors.h"
#include "mem.h"
#include "ansi_colors.h"
#include "string_utils.h"

#define MAX_BUF 1024

struct _PrintTime {
  const char* file_name;
  FILE* file;
  ArrayAlt* header;
  ArrayAlt* data;
};

static void PrintTime_save_dictionary(PrintTime* pt, const char* padding) {
  for_each(ArrayAlt_it(pt->data), ^(void* elem) {
    KeyValue kv = *(KeyValue*)elem;
    fprintf(pt->file, "%s%s: %lf\n", padding, (char*) kv.key, DoubleContainer_get((DoubleContainer*)kv.value));
  });
}

static void PrintTime_save_header(PrintTime* pt) {
  fprintf(pt->file, "-\n");
  for_each(ArrayAlt_it(pt->header), ^(void* elem) {
    KeyValue kv = *(KeyValue*)elem;
    fprintf(pt->file, "  %s: %s\n", (char*) kv.key, (const char*) kv.value);
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
    Error_raise(Error_new(ERROR_GENERIC, "localtime failed"));
  }

  if(strftime(buf, 1024, "%a %b %d %H:%M:%S %Y", tmp) == 0) {
    Error_raise(Error_new(ERROR_GENERIC, "Cannot format time"));
  }

  return buf;
}

PrintTime* PrintTime_new(const char* out_file) {
  if(out_file==NULL) {
    out_file = "timings.yml";
  }

  PrintTime* pt = (PrintTime*) Mem_alloc(sizeof(struct _PrintTime));
  pt->file_name = out_file;
  pt->file = NULL;

  pt->header = ArrayAlt_new(20, sizeof(KeyValue));
  pt->data = ArrayAlt_new(20, sizeof(KeyValue));

  PrintTime_add_header(pt, "date", get_date_string());

  return pt;
}



void PrintTime_add_header(PrintTime* pt, const char* key, const char* value) {
  KeyValue kv = { .key = Mem_strdup(key), .value = Mem_strdup(value) };
  ArrayAlt_add(pt->header, &kv);
}

void PrintTime_free(PrintTime* pt) {
  for_each(ArrayAlt_it(pt->header), ^(void* elem) {
    KeyValue kv = *(KeyValue*) elem;
    Mem_free(kv.key);
    Mem_free(kv.value);
  });
  ArrayAlt_free(pt->header);

  for_each(ArrayAlt_it(pt->data), ^(void* elem) {
    KeyValue kv = *(KeyValue*) elem;
    Mem_free(kv.key);
    DoubleContainer_free((DoubleContainer*) kv.value);
  });
  ArrayAlt_free(pt->data);

  Mem_free(pt);
}


double PrintTime_print(PrintTime* pt, char* label, void(^fun)(void)) {
 double result;
 char buf[MAX_BUF];

 printf("%s\n", String_head_line(buf, label, '=', MAX_BUF));

 clock_t start = clock();
 fun();
 clock_t end = clock();
 result = ((double)end-start) / CLOCKS_PER_SEC;

 printf(BWHT "%s\n" reset, String_line_with_termsize(buf, '-', MAX_BUF));
 printf(BWHT "time:" BRED "%10.2lf" reset " secs\n" , result);
 printf(BWHT "%s\n\n" reset, String_line_with_termsize(buf, '=', MAX_BUF));

 KeyValue kv = { .key = Mem_strdup(label), .value = DoubleContainer_new(result) };
 ArrayAlt_add(pt->data, &kv);

 return result;
}

void PrintTime_save(PrintTime* pt) {

  pt->file = fopen(pt->file_name, "a");
  if(!pt->file) {
    Error_raise(Error_new(ERROR_FILE_OPENING, "Cannot open output file - %s", strerror(errno)));
  }

  if(flock(fileno(pt->file), LOCK_EX) != 0) {
    Error* error = Error_new(ERROR_FILE_LOCKING, "Unable to lock file %s -- %s", pt->file_name, strerror(errno));
    fclose(pt->file);
    Error_raise(error);
  }

  PrintTime_save_header(pt);
  PrintTime_save_dictionary(pt, "    ");

  if(flock(fileno(pt->file), LOCK_UN) != 0) {
    Error* error = Error_new(ERROR_FILE_LOCKING, "Cannot unlock file %s -- %s", pt->file_name, strerror(errno));
    fclose(pt->file);
    Error_raise(error);
  }

  fclose(pt->file);
}
