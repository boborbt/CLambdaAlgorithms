#include "array_g.h"
#include <assert.h>
#include <string.h>

typedef unsigned char uchar;

void* at_g(const void* array, int pos, int size) {
  const uchar* carray = (const uchar*) array;
  return (void*) &carray[pos*size];
}

void cp_g(void* dst, const void* src, int size) {
    memcpy(dst, src, size);
}

void swap_g(void* el1, void* el2, int size) {
  assert(size <= 1024);
  uchar buf[1024];

  cp_g(buf, el1, size);
  cp_g(el1, el2, size);
  cp_g(el2, buf, size);
}
