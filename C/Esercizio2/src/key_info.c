#include "key_info.h"
#include <stdlib.h>

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
