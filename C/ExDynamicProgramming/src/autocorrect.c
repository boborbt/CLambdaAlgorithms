#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "editing_distance.h"
#include "array.h"
#include "iterator_functions.h"
#include "print_time.h"
#include "errors.h"


typedef struct {
  unsigned long distance;
  const char* closest_match;
} EDResult;

static Array* load_strings(const char* filename) {
  FILE* file = fopen(filename, "r");
  if(file==NULL) {
    Error_raise( Error_new( ERROR_FILE_OPENING, "Cannot open file %s for reading, reason: %s", filename, strerror(errno)));
  }


  Array* array = Array_new(1000);

  while(!feof(file)) {
    char buf[2048];
    fscanf(file, "%s", buf);

    if(!feof(file)) {
      Array_add(array, strdup(buf));
    }
  }

  fclose(file);

  return array;
}

static EDResult find_closest_match(const char* word, Array* word_list) {
  __block EDResult result;
  result.distance = ULONG_MAX;
  result.closest_match = NULL;

  find_first(Array_it(word_list), ^(void* obj) {
    const char* match_candidate = (char*) obj;
    unsigned long distance = editing_distance(word, match_candidate);

    if(distance < result.distance) {
      result.distance = distance;
      result.closest_match = match_candidate;
    }

    if(result.distance == 0) {
      return 1;
    }

    return 0;
  });

  return result;
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

  fclose(file);

  buf[strlen(buf)-1] = '\0'; // removing trailing \n
  return buf;
}

static PrintTime* init_print_time(char const* argv[]) {
  PrintTime* pt = PrintTime_new(NULL);

  PrintTime_add_header(pt,"Exercise", "ExDynamicProgramming");
  PrintTime_add_header(pt, "invocation", argv[0]);
  PrintTime_add_header(pt, "compilation_flags", get_compilation_flags());

  return pt;
}

int main(int argc, char const *argv[]) {
  if( argc < 3 ) {
    printf("Usage: autocorrect <wordlist> <text>\n");
    exit(1);
  }

  PrintTime* pt = init_print_time(argv);

  __block Array* word_list;
  __block Array* text;
  PrintTime_print(pt, "Dataset_load", ^() {
    printf("Loading datasets...\n");
    word_list = load_strings(argv[1]);
    text = load_strings(argv[2]);
    printf("Done!\n");
  });

  PrintTime_print(pt, "Finding matches", ^() {
    printf("Finding matches...\n");
    for_each(Array_it(text), ^(void* obj){
      const char* word = (char*) obj;
      printf("analyzing word: %s\n", word);

      EDResult result = find_closest_match(word, word_list);
      if(result.distance != 0) {
        printf("word %s is mispelled, the closest match is %s\n", word, result.closest_match);
      }
    });
    printf("Done!\n");
  });

  Array_free(word_list);
  Array_free(text);

  PrintTime_save(pt);
  PrintTime_free(pt);

  return 0;
}
