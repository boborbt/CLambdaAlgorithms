#include <stdlib.h>
#include <string.h>

#include "keys.h"
#include "mem.h"

struct _KeyInfo {
  KIComparator comparator;
  KIHash hash;
};

KeyInfo* KeyInfo_new( KIComparator c, KIHash h) {
  KeyInfo* result = (KeyInfo*) Mem_alloc(sizeof(struct _KeyInfo));
  result->comparator = c;
  result->hash = h;
  return result;
}

KIComparator KeyInfo_comparator(KeyInfo* keyInfo) {
  return keyInfo->comparator;
}

KIHash KeyInfo_hash(KeyInfo* keyInfo) {
  return keyInfo->hash;
}

void KeyInfo_free(KeyInfo* keyInfo) {
  Mem_free(keyInfo);
}

int Key_string_compare(const void* e1, const void* e2) {
  return strcmp((const char*) e1, (const char*) e2);
}

int Key_int_compare(const void* e1, const void* e2) {
  int i1 = *(const int*) e1;
  int i2 = *(const int*) e2;

  if( i1 < i2 ) return -1;
  if( i1 > i2 ) return 1;
  return 0;
}

int Key_long_compare(const void* e1, const void* e2) {
  long int i1 = *(const long int*) e1;
  long int i2 = *(const long int*) e2;

  if( i1 < i2 ) return -1;
  if( i1 > i2 ) return 1;
  return 0;
}

int Key_double_compare(const void* e1, const void* e2) {
  double d1 = *(const double*) e1;
  double d2 = *(const double*) e2;

  if( d1 < d2 ) return -1;
  if( d1 > d2 ) return 1;
  return 0;
}

size_t Key_string_hash(const void* e1) {
  const char* str = (const char*) e1;

  size_t h = 0;
  size_t len = strlen(str);
  for(size_t i=0; i<len; ++i) {
    size_t highorder = h & 0xf8000000;
    h = h << 5;                    // shift h left by 5 bits
    h = h ^ (highorder >> 27);     // move the highorder 5 bits to the low-order
    h = h ^ (size_t)str[i];  // XOR h and ki
  }
  return h;
}

size_t Key_int_hash(const void* e) {
  int value = *(const int*)e;
  return (size_t)(value * (value+3));
}

size_t Key_double_hash(const void* e) {
  double value = *(const double*) e;
  return (size_t) (value * (value+3));
}
