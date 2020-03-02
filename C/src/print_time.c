#include "print_time.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/file.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

#include "double_container.h"
#include "array_alt.h"
#include "iterator_functions.h"
#include "errors.h"
#include "mem.h"
#include "ansi_colors.h"

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
    fprintf(pt->file, "%s%s: %lf\n", padding, kv.key, DoubleContainer_get((DoubleContainer*)kv.value));
  });
}

static void PrintTime_save_header(PrintTime* pt) {
  fprintf(pt->file, "-\n");
  for_each(ArrayAlt_it(pt->header), ^(void* elem) {
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

static char* strline(char* str, char c, unsigned long n)  {
  unsigned long i;
  for(i=0; i<(n-1); ++i) {
    str[i] = c;
  }

  str[i++] = '\0';

  return str;
}

static char* sep_line(char* buf, char c, unsigned long n) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  unsigned long i;
  for(i=0; i<w.ws_col && i<(n-1); ++i) {
    buf[i] = c;
  }
  buf[i++] = '\0';

  return buf;
}

#define SPACE_LEFT ((n - 1) - strlen(hl))
#define min(a,b) ((a) < (b) ? (a) : (b))
/**
 * Constructs and returns a string containing the given label surrounded by
 * square brackets and centered in a field of length equal to the current 
 * terminal line size (or BUF_SIZE if the line is larger than BUF_SIZE). 
 * At each side of the label characters are filled with the given c symbol. 
 * ANSI color characters are also inserted so to make everything bold and white
 * with the exception of the label which is rendered in bold and green.
 * 
 * # parameters:
 * 
 * - hl: a character array with enough space to hold at least n characters
 * - label: the label to be rendered
 * - c: the character to be repeated at each side of the label
 * - n: size of the hl buffer.
 * 
 * # result (example): 
 * 
 * head_line(hl, "test", '=', 100) and assuming a terminal line length of 22 
 * would copy the string "========[test]========" into the hl buffer.
 */
static char* head_line(char* hl, char* label, char c, unsigned long n) {
  static char buf[MAX_BUF] = "";

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  if(strlen(label)>=w.ws_col-6) {
    return label;
  }

  unsigned long w_left = min((w.ws_col / 2) - (strlen(label) / 2), MAX_BUF);

  strncpy(hl, BWHT, n);
  strncat(hl, strline(buf, c, w_left) , SPACE_LEFT);
  strncat(hl, "[", SPACE_LEFT);
  strncat(hl, BGRN, SPACE_LEFT);
  strncat(hl, label, SPACE_LEFT);
  strncat(hl, BWHT, SPACE_LEFT);
  strncat(hl, "]", SPACE_LEFT);

  unsigned long w_end = w.ws_col - (w_left + strlen(label));
  strncat(hl, strline(buf, c, w_end), SPACE_LEFT);
  strncat(hl, reset, SPACE_LEFT);

  return hl;
}
#undef SPACE_LEFT
#undef min


double PrintTime_print(PrintTime* pt, char* label, void(^fun)(void)) {
 double result;
 char buf[MAX_BUF];

 printf("%s\n", head_line(buf, label, '=', MAX_BUF));
 clock_t start = clock();
 fun();
 clock_t end = clock();

 result = ((double)end-start) / CLOCKS_PER_SEC;

 
 printf(BWHT "%s\n" reset, sep_line(buf, '-', MAX_BUF));
 printf(BWHT "time:" BRED "%10.2lf" reset " secs\n" , result);
 printf(BWHT "%s\n\n" reset, sep_line(buf, '=', MAX_BUF));

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
