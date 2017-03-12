#pragma once

// UnionFindSet* opaque type
typedef struct _UnionFindSet UnionFindSet;

// Creates a new singleton set containing the given object
UnionFindSet* UnionFindSet_new(void*);

// Merges s1 and s2 into one set
void UnionFindSet_union(UnionFindSet* s1, UnionFindSet* s2);

// Returns 1 iff s1 and s2 are the same set
int UnionFindSet_same(UnionFindSet* s1, UnionFindSet* s2);

// Returns the representative of the given set. The representative is such
// that if s1 and s2 are two distinct elements belonging to a given set, and
// r(s1), r(s2) are their representative, then r(s1) == r(s2) iff s1 and s2
// belongs to the same set.
UnionFindSet* UnionFindSet_find(UnionFindSet* set);

// Returns the object wrapped in the given set element
void* UnionFindSet_get(UnionFindSet* set);

// Frees the given UnionFindSet
void UnionFindSet_free(UnionFindSet* set);
