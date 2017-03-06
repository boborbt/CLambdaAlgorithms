#include "string_utils.h"
#include <string.h>

Array String_split(char* string, char delim) {
  Array result = Array_new(10, sizeof(char*));
  size_t len = strlen(string);

  char* buf = (char*) malloc(sizeof(char) * len + 1);
  size_t pos = 0;
  size_t buf_pos = 0;

  while(pos < len) {
    if(string[pos] == delim) {
      buf[buf_pos] = '\0';
      char* piece = strdup(buf);
      Array_add(result, &piece);
      buf_pos = 0;
      pos += 1;
      continue;
    }

    buf[buf_pos++] = string[pos++];
  }

  if(buf_pos!=0) {
    buf[buf_pos] = '\0';
    char* piece = strdup(buf);
    Array_add(result, &piece);
  }

  free(buf);
  return result;
}



char* String_join(Array array, char delim) {
  __block size_t total_len = 0;
  char* delim_str = (char*) malloc(sizeof(char)*2);
  delim_str[0] = delim;
  delim_str[1] = '\0';

  for_each(Array_it(array), ^(void* elem) {
    char* piece = *(char**) elem;
    total_len += strlen(piece)+1;
  });

  // we will be wasting one character. We keep this way
  // for clarity of implementation (in this way we can simply
  // add "," to each piece and remove the last one at the end)
  char* buf = (char*) malloc(sizeof(char) * (total_len + 1));
  buf[0] = '\0';

  for_each(Array_it(array), ^(void* elem) {
    char* piece = *(char**) elem;
    strcat(buf, piece);
    strcat(buf, delim_str);
  });

  buf[total_len-1] = '\0';

  free(delim_str);

  return buf;
}
