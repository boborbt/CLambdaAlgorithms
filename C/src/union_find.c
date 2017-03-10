#include "union_find.h"
#include <stdlib.h>

#include "mem.h"

struct _UnionFindSet {
  void* elem;
  struct _UnionFindSet* parent;
};

UnionFindSet UnionFindSet_new(void* elem) {
  UnionFindSet result = (UnionFindSet) Mem_alloc(sizeof(struct _UnionFindSet));
  result->elem = elem;
  result->parent = NULL;
  return result;
}

void UnionFindSet_union(UnionFindSet s1, UnionFindSet s2) {
  UnionFindSet s1_guard = UnionFindSet_find(s1);
  UnionFindSet s2_guard = UnionFindSet_find(s2);
  if(s1_guard == s2_guard) {
    return;
  }

  s1_guard->parent = s2_guard;
}

UnionFindSet UnionFindSet_find(UnionFindSet set) {
  if(set->parent == NULL) {
    return set;
  }

  set->parent = UnionFindSet_find(set->parent);
  return set->parent;
}

void* UnionFindSet_get(UnionFindSet set) {
  return set->elem;
}

void UnionFindSet_free(UnionFindSet set) {
  Mem_free(set);
}

int UnionFindSet_same(UnionFindSet set1, UnionFindSet set2) {
  return UnionFindSet_find(set1) == UnionFindSet_find(set2);
}
