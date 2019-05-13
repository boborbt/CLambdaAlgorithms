#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include "editing_distance.h"
#include "array.h"
#include "iterator_functions.h"
#include "basic_iterators.h"
#include "print_time.h"
#include "errors.h"
#include "mem.h"
#include "string_utils.h"


typedef struct {
  long distance;
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

// Compares e1 and e2 first on the basis of their lenght and then on the basis of their 
// lexicographic order.
static int alphalen_cmp(const void* e1, const void* e2) {
  const char *s1 = (const char *)e1, *s2 = (const char *)e2;
  size_t len1 = strlen(s1), len2 = strlen(s2);

  if (len1 == len2) {
    return strcmp(s1, s2);
  }

  if (len1 < len2) {
    return -1;
  }

  return 1;
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

static Array* load_strings(const char* filename, char delim) {
  Array* array = Array_new(1000);

  for_each(TextFile_it(filename, delim), ^(void* obj) {
    char* buf = (char*) obj;
    str_strip_chars(buf);
    str_tolower(buf);
    Array_add(array, strdup(buf));
  });


  if(delim == ' ') {
    printf("array:\n");
    for_each(Array_it(array), ^(void* obj) {
      printf("%s ", (char*) obj);
    });
    printf("\n");
  }

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

/*
 * Search for an exact match of word in word_list. If it finds one, then it returns ULONG_MAX.
 * Otherwise it returns the position of the nearest match.
 */

static size_t find_exact_match(const char* word, Array* word_list) {
  size_t binsearch_result = binsearch_approx(
      Array_it(word_list), word, ^(const void *lhs, const void *rhs) {
        return alphalen_cmp(lhs, rhs);
      });

  const char* other = Array_at(word_list, binsearch_result);

  if (strcmp(word, other) == 0) {
    return ULONG_MAX;
  } else {
    return binsearch_result;
  }
}

/*
 * Returns the distance between the given two unsigned integers (i.e., |u1 - u2|).
 */

static long udiff(unsigned long u1, unsigned long u2) {
  if(u1 >= u2) {
    return (long) (u1 - u2);
  }

  return (long) (u2 - u1);
}

/*
 * Starting from start_index and using the increment function to update the index, it searches
 * the word_list for the best possible matches to the given word. Every time it finds a word with
 * the same distance as the best matches in result, then it adds the word the best matches. 
 * Every time it finds a word with a lower distance, it reset the best matches array and insert
 * the word as the best match.
 * 
 * It stops the search when the index reaches the given limit, or when it is no longer possible
 * to improve the results. Presently, it assumes that the word array is sorted according to the
 * word lengths and stops the result when the difference in length between word and the current
 * candidate is larger than the current distance.
 */

static void find_with_increment(const char *word, Array *word_list,
                           long (editing_distance)(const char *, const char *, long),
                           size_t start_index, size_t limit,
                           EDResult* result,
                           size_t (increment)(size_t)) {
  size_t index = start_index;
  char *match_candidate = NULL;
  size_t word_len = strlen(word);

  if (index != limit) {
    match_candidate = (char*) Array_at(word_list, index);
  }

  while (index != limit && udiff(word_len,strlen(match_candidate)) <= result->distance ) {
    long distance = (long) editing_distance(word, match_candidate, result->distance);

    if (distance == result->distance) {
      Array_add(result->closest_matches, match_candidate);
    }

    if (distance < result->distance) {
      Array_free(result->closest_matches);
      result->closest_matches = Array_new(10);
      Array_add(result->closest_matches, match_candidate);
      result->distance = distance;
    }

    index = increment(index);
    if (index != limit) {
      match_candidate = (char*) Array_at(word_list, index);
    }
  }
}

static size_t inc_index(size_t index) {
  return index + 1;
}

static size_t dec_index(size_t index) {
  return index - 1;
}

static EDResult find_closest_match(const char* word, Array* word_list, long (editing_distance)(const char*, const char*, long)) {
  __block EDResult result;
  result.distance = 0;
  result.closest_matches = NULL;

  size_t match_index = find_exact_match(word, word_list);

  if(match_index == ULONG_MAX) {
    return result;
  }

  result.closest_matches = Array_new(10);
  Array_add(result.closest_matches, Array_at(word_list, match_index));
  result.distance = (long) editing_distance(word, (const char*) Array_at(word_list, match_index), LONG_MAX);

  find_with_increment(word, word_list, editing_distance, match_index + 1,Array_size(word_list) - 1, &result, inc_index);
  find_with_increment(word, word_list, editing_distance, match_index - 1, (size_t)-1, &result, dec_index);

  return result;
}


static char* get_compilation_flags() {
  static char buf[4096];
  Array* strings = Array_new(5);

  for_each(TextFile_it("Makefile.vars",'\n'), ^(void* obj){
    char* line = (char*) obj;
    if(line[0] == '\0' || line[0] == '#') {
      return;
    }

    if(strstr(line, "CFLAGS")!=NULL) {
      Array_add(strings, Mem_strdup(line));
    }
  });

  char* result = String_join(strings, ' ');
  strcpy(buf, result);

  Mem_free(result);

  free_contents(Array_it(strings));

  Array_free(strings);

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
    word_list = load_strings(argv[1],'\n');
    text = load_strings(argv[2], ' ');
    printf("Done!\n");
  });

  PrintTime_print(pt, "Sorting dictionary", ^() {
    printf("Sorting dictionary...");
    sort(Array_it(word_list), ^(const void* e1, const void* e2) {
      return alphalen_cmp(e1,e2);
    });
  });

  PrintTime_print(pt, "Finding matches", ^() {
    printf("Finding matches...\n");
    for_each(Array_it(text), ^(void* obj){
      const char* word = (char*) obj;
      printf("analyzing word: %s\n", word);

      EDResult result = find_closest_match(word, word_list, editing_distance);
      if(result.distance != 0) {
        printf("word %s is mispelled, the closest matches (dist: %ld) are:\n", word, result.distance);
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
