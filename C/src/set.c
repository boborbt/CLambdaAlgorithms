#include "set.h"
#include "dictionary.h"
#include "iterator_functions.h"


// Constructor/destructor
Set* Set_new(KeyInfo* ki) {
  return Dictionary_new(ki);
}


void Set_free(Set* set) {
  Dictionary_free(set);
}

// Inserts a new object into the set. Does nothing if the object is already present.
void Set_insert(Set* set, void* obj) {
  if(!Dictionary_get(set, obj, NULL)) {
    Dictionary_set(set, obj, NULL);
  }
}

KeyInfo* Set_key_info(Set* set) {
  return Dictionary_key_info(set);
}

size_t Set_size(Set* set) {
  return Dictionary_size(set);
}

void Set_remove(Set* set, void* obj) {
  Dictionary_delete(set, obj);
}

// Returns 1 if the given object belongs to the set; 0 otherwise
int Set_contains(Set* set, void* obj) {
  return Dictionary_get(set, obj, NULL);
}


// Returns a newly built set containing the union of the given two sets
// The new set needs to be dealloced by the user
// Precondition: s1 and s2 must be based on the same KeyInfo
Set* Set_union(Set* s1, Set* s2) {
  __block Set* result = Set_new(Set_key_info(s1));

  for_each(Set_it(s1), ^(void* obj) {
    Set_insert(result, obj);
  });

  for_each(Set_it(s2), ^(void* obj) {
    Set_insert(result, obj);
  });

  return result;
}

// Returns a newly built set containg the intersection fo the given two sets
// The new set needs to be dealloced by the user
// Precondition: s1 and s2 must be based on the same KeyInfo
Set* Set_intersection(Set* s1, Set* s2) {
  __block Set* result = Set_new(Set_key_info(s1));

  for_each(Set_it(s1), ^(void* obj) {
    if(Set_contains(s2, obj)) {
      Set_insert(result, obj);
    }
  });

  return result;
}

// Returns a newly built set containing all objects in s1 not included in s2
// The new set needs to be dealloced by the user
// Precondition: s1 and s2 must be based on the same KeyInfo
Set* Set_difference(Set* s1, Set* s2) {
  __block Set* result = Set_new(Set_key_info(s1));

  for_each(Set_it(s1), ^(void* obj) {
    if(!Set_contains(s2, obj)) {
      Set_insert(result, obj);
    }
  });

  return result;
}

// Inplace union of s1 and s2. After the method call s1 will also contain all objects
// s2.
// Precondition: s1 and s2 must be based on the same KeyInfo
void Set_inplace_union(Set* s1, Set* s2) {
  for_each(Set_it(s2), ^(void* obj) {
    Set_insert(s1, obj);
  });
}

// Inplace intersection of s1 and s2. After the method call s1 will contain only objects
// that are both in s1 and s2.
// Precondition: s1 and s2 must be based on the same KeyInfo
void Set_inplace_intersect(Set* s1, Set* s2) {
  for_each(Set_it(s1), ^(void* obj) {
    if(!Set_contains(s2, obj)) {
      Set_remove(s1, obj);
    }
  });
}

// Inplace difference of s1 and s2. After the method call s1 will not contain any object
// in s2.
// Precondition: s1 and s2 must be based on the same KeyInfo
void Set_inplace_difference(Set* s1, Set* s2) {
  for_each(Set_it(s1), ^(void* obj) {
    if(Set_contains(s2, obj)) {
      Set_remove(s1, obj);
    }
  });
}

int Set_equals(Set* s1, Set* s2) {
  if(Set_size(s1) != Set_size(s2)) {
    return 0;
  }

  void* found_obj = find_first(Set_it(s1), ^(void* obj) {
    return !Set_contains(s2, obj);
  });

  return found_obj == NULL;
}


Iterator Set_it(Set* set) {
  return Dictionary_key_it(set);
}
