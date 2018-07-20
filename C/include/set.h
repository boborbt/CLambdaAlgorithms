#pragma once

#include "dictionary.h"

typedef Dictionary Set;

// Constructor/destructor
Set* Set_new(KeyInfo*);
void Set_free(Set*);

// Returs the cardinality of the set
size_t Set_size(Set* set);

// Returns the key info used to build this set
KeyInfo* Set_key_info(Set*);

// Inserts a new object into the set. Does nothing if the object is already present.
void Set_insert(Set*, void*);

// Returns 1 if the given object belongs to the set; 0 otherwise
int Set_contains(Set*, void*);

void Set_remove(Set*, void*);

// Returns a newly built set containing the union of the given two sets
// The new set needs to be dealloced by the user
// Precondition: s1 and s2 must be based on the same KeyInfo
Set* Set_union(Set*, Set*);

// Returns a newly built set containg the intersection fo the given two sets
// The new set needs to be dealloced by the user
// Precondition: s1 and s2 must be based on the same KeyInfo
Set* Set_intersection(Set*, Set*);

// Returns a newly built set containing all objects in s1 not included in s2
// The new set needs to be dealloced by the user
// Precondition: s1 and s2 must be based on the same KeyInfo
Set* Set_difference(Set* s1, Set* s2);

// Inplace union of s1 and s2. After the method call s1 will also contain all objects
// s2.
// Precondition: s1 and s2 must be based on the same KeyInfo
void Set_inplace_union(Set* s1, Set* s2);

// Inplace intersection of s1 and s2. After the method call s1 will contain only objects
// that are both in s1 and s2.
// Precondition: s1 and s2 must be based on the same KeyInfo
void Set_inplace_intersect(Set* s1, Set* s2);

// Inplace difference of s1 and s2. After the method call s1 will not contain any object
// in s2.
// Precondition: s1 and s2 must be based on the same KeyInfo
void Set_inplace_difference(Set* s1, Set* s2);

// Returns 1 if s1 and s2 are the same set (i.e., they contains the same objects)
int Set_equals(Set* s1, Set* s2);

// Iterator interface
Iterator Set_it(Set*);
