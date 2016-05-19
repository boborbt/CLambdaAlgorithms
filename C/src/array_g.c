#include "array_g.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uchar;

#ifndef ARRAY_G_SWAP_BUFFER_SIZE
#define ARRAY_G_SWAP_BUFFER_SIZE 4096
#endif

void* at_g(void* array, size_t pos, size_t size) {
  uchar* carray = (uchar*) array;
  return (void*) (carray + pos*size);
}

void cp_g(void* dst, const void* src, size_t size) {
    memcpy(dst, src, size);
}

void swap_g(void* el1, void* el2, size_t size) {
  assert(size <= ARRAY_G_SWAP_BUFFER_SIZE);
  static uchar SWAP_BUFFER[ARRAY_G_SWAP_BUFFER_SIZE];

  cp_g(SWAP_BUFFER, el1, size);
  cp_g(el1, el2, size);
  cp_g(el2, SWAP_BUFFER, size);
}
