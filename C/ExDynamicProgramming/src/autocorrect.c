#include <stdio.h>
#include <ctype.h>
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
  Array* closest_matches;
} EDResult;


static int in_bad_char_set(char ch) {
  switch(ch) {
    case ',':
    case '.':
    case '\'':
    case '\n':
    case ':':
    case ';':
      return 1;
    default:
      return 0;
  }
}

static void str_strip_chars(char* buf) {
  int index = 0;

  for(int i=0; buf[i]!='\0'; ++i) {
    if(!in_bad_char_set(buf[i])) {
      buf[index++] = buf[i];
    }
  }

  buf[index] = '\0';
}

static void str_tolower(char* buf) {
  for(int i=0; buf[i] != '\0'; ++i) {
    buf[i] = (char) tolower(buf[i]);
  }
}

static Array* load_strings(const char* filename) {
  FILE* file = fopen(filename, "r");
  if(file==NULL) {
    Error_raise( Error_new( ERROR_FILE_OPENING, "Cannot open file %s for reading, reason: %s", filename, strerror(errno)));
  }


  Array* array = Array_new(1000);

  while(!feof(file)) {
    char buf[2048];
    if(fscanf(file, "%s", buf)==1) {
      str_strip_chars(buf);
      str_tolower(buf);
      Array_add(array, strdup(buf));
    }
  }

  fclose(file);

  return array;
}

// static unsigned long min(unsigned long v1, unsigned long v2) {
//   if(v1 < v2) {
//     return v1;
//   } else {
//     return v2;
//   }
// }

// // Recursive function to compute the editing distance between string1 and string2.
// static unsigned long editing_distance_dummy(const char* string1, const char* string2) {
//   if(string1[0] == '\0') {
//     return strlen(string2);
//   }
//
//   if(string2[0] == '\0') {
//     return strlen(string1);
//   }
//
//   unsigned long del_c = editing_distance_dummy(string1, string2+1) + 1;
//   unsigned long copy_c = editing_distance_dummy(string1+1, string2) + 1;
//   if(string1[0] == string2[0]) {
//     return min(del_c, min(copy_c, editing_distance_dummy(string1+1, string2+1)));
//   } else {
//     return min(del_c, copy_c);
//   }
// }

static EDResult find_closest_match(const char* word, Array* word_list, unsigned long (editing_distance)(const char*, const char*)) {
  __block EDResult result;
  result.distance = ULONG_MAX;
  result.closest_matches = NULL;

  unsigned long binsearch_result = Array_binsearch(word_list, ^(const void* obj) {
    const char* str = (const char*) obj;
    return strcmp(word, str);
  });

  if( binsearch_result != ULONG_MAX) {
    result.distance = 0;
    return result;
  }

  result.closest_matches = Array_new(10);

  for_each(Array_it(word_list), ^(void* obj) {
    const char* match_candidate = (char*) obj;
    unsigned long distance = editing_distance(word, match_candidate);

    if(distance == result.distance) {
      Array_add(result.closest_matches, obj);
      return;
    }

    if(distance < result.distance) {
      Array_free(result.closest_matches);
      result.closest_matches = Array_new(10);
      Array_add(result.closest_matches, obj);
      result.distance = distance;
    }
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

      EDResult result = find_closest_match(word, word_list, editing_distance);
      if(result.distance != 0) {
        printf("word %s is mispelled, the closest matches are:\n", word);
        for_each(Array_it(result.closest_matches), ^(void* correction) {
          printf("   %s\n", (const char*) correction);
        });
      }

      if(result.closest_matches!=NULL) {
        Array_free(result.closest_matches);
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
