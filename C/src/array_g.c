#include "array_g.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uchar;

#define SWAP_BUFFER_SIZE 4096
static uchar SWAP_BUFFER[SWAP_BUFFER_SIZE];


void* at_g(void* array, size_t pos, size_t size) {
  uchar* carray = (uchar*) array;
  return (void*) &carray[pos*size];
}

void cp_g(void* dst, const void* src, size_t size) {
    memcpy(dst, src, size);
}

void swap_g(void* el1, void* el2, size_t size) {
  assert(size <= SWAP_BUFFER_SIZE);

  cp_g(SWAP_BUFFER, el1, size);
  cp_g(el1, el2, size);
  cp_g(el2, SWAP_BUFFER, size);
}
