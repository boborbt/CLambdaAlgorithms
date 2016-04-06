#include "key_info.h"
#include <stdlib.h>
#include <string.h>

struct _KeyInfo {
  KIComparator comparator;
  KIHash hash;
};

KeyInfo KeyInfo_new( KIComparator c, KIHash h) {
  KeyInfo result = (KeyInfo) malloc(sizeof(struct _KeyInfo));
  result->comparator = c;
  result->hash = h;
  return result;
}

KIComparator KeyInfo_comparator(KeyInfo keyInfo) {
  return keyInfo->comparator;
}

KIHash KeyInfo_hash(KeyInfo keyInfo) {
  return keyInfo->hash;
}

void KeyInfo_free(KeyInfo keyInfo) {
  free(keyInfo);
}

int KeyInfo_string_compare(const void* e1, const void* e2) {
  return strcmp((const char*) e1, (const char*) e2);
}

int KeyInfo_int_compare(const void* e1, const void* e2) {
  int i1 = *(const int*) e1;
  int i2 = *(const int*) e2;

  if( i1 < i2 ) return -1;
  if( i1 > i2 ) return 1;
  return 0;
}

int KeyInfo_double_compare(const void* e1, const void* e2) {
  double d1 = *(const double*) e1;
  double d2 = *(const double*) e2;

  if( d1 < d2 ) return -1;
  if( d1 > d2 ) return 1;
  return 0;
}

unsigned int KeyInfo_string_hash(const void* e1) {
  const char* str = (const char*) e1;

  unsigned int h = 0;
  size_t len = strlen(str);
  for(unsigned int i=0; i<len; ++i) {
    unsigned int highorder = h & 0xf8000000;
    h = h << 5;                    // shift h left by 5 bits
    h = h ^ (highorder >> 27);     // move the highorder 5 bits to the low-order
    h = h ^ (unsigned int)str[i];  // XOR h and ki
  }
  return h;
}

unsigned int KeyInfo_int_hash(const void* e) {
  int value = *(const int*)e;
  return (unsigned int)(value * (value+3));
}

unsigned int KeyInfo_double_hash(const void* e) {
  double value = *(const double*) e;
  return (unsigned int) (value * (value+3));
}
