#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>

#include "iterator_functions.h"
#include "mem.h"
#include "string_utils.h"
#include "ansi_colors.h"

#define MAX_BUF 1024

Array* String_split(char* string, char delim) {
  Array* result = Array_new(10);
  size_t len = strlen(string);

  char* buf = (char*) Mem_alloc(sizeof(char) * len + 1);
  size_t pos = 0;
  size_t buf_pos = 0;

  while(pos < len) {
    if(string[pos] == delim) {
      buf[buf_pos] = '\0';
      char* piece = Mem_strdup(buf);
      Array_add(result, piece);
      buf_pos = 0;
      pos += 1;
      continue;
    }

    buf[buf_pos++] = string[pos++];
  }

  if(buf_pos!=0) {
    buf[buf_pos] = '\0';
    char* piece = Mem_strdup(buf);
    Array_add(result, piece);
  }

  Mem_free(buf);
  return result;
}



char* String_join(Array* array, char delim) {
  __block size_t total_len = 0;
  char* delim_str = (char*) Mem_alloc(sizeof(char)*2);
  delim_str[0] = delim;
  delim_str[1] = '\0';

  for_each(Array_it(array), ^(void* elem) {
    char* piece = elem;
    total_len += strlen(piece)+1;
  });

  // we will be wasting one character. We keep this way
  // for clarity of implementation (in this way we can simply
  // add "," to each piece and remove the last one at the end)
  char* buf = (char*) Mem_alloc(sizeof(char) * (total_len + 1));
  buf[0] = '\0';

  for_each(Array_it(array), ^(void* elem) {
    char* piece = elem;
    strcat(buf, piece);
    strcat(buf, delim_str);
  });

  buf[total_len-1] = '\0';

  Mem_free(delim_str);

  return buf;
}

char* String_repeatchar(char* str, char c, unsigned long n) {
  unsigned long i;
  for (i = 0; i < (n - 1); ++i) {
    str[i] = c;
  }

  str[i++] = '\0';

  return str;
}

#define min(a, b) ((a) < (b) ? (a) : (b))
char* String_line_with_termsize(char* buf, char c, unsigned long n) {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  n = min(w.ws_col, n-1);

  String_repeatchar(buf, c,  n);

  return buf;
}

#define SPACE_LEFT ((n - 1) - strlen(hl))

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
char* String_head_line(char* hl, char* label, char c, unsigned long n) {
  static char buf[MAX_BUF] = "";

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  if (strlen(label) >= w.ws_col - 6) {
    return label;
  }

  unsigned long w_left = min((w.ws_col / 2) - (strlen(label) / 2), MAX_BUF);

  strncpy(hl, BWHT, n);
  strncat(hl, String_repeatchar(buf, c, w_left), SPACE_LEFT);
  strncat(hl, "[", SPACE_LEFT);
  strncat(hl, BGRN, SPACE_LEFT);
  strncat(hl, label, SPACE_LEFT);
  strncat(hl, BWHT, SPACE_LEFT);
  strncat(hl, "]", SPACE_LEFT);

  unsigned long w_end = w.ws_col - (w_left + strlen(label));
  strncat(hl, String_repeatchar(buf, c, w_end), SPACE_LEFT);
  strncat(hl, reset, SPACE_LEFT);

  return hl;
}
#undef SPACE_LEFT
#undef min
