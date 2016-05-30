#pragma once

typedef struct _UnionFindSet* UnionFindSet;

UnionFindSet UnionFindSet_new(void*);
void UnionFindSet_union(UnionFindSet s1, UnionFindSet s2);
int UnionFindSet_same(UnionFindSet elem, UnionFindSet set);

UnionFindSet UnionFindSet_find(UnionFindSet set);
void* UnionFindSet_get(UnionFindSet set);
void UnionFindSet_free(UnionFindSet set);
