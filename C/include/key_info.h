#pragma once

/*
 * KeyInfo maintains stores a pair of functions to compare and
 * create hashes of given data.
 */

typedef struct _KeyInfo* KeyInfo;
typedef int (*KIComparator)(const void*, const void*);
typedef unsigned int (*KIHash)(const void*);

/* Constructor and destructor*/
KeyInfo KeyInfo_new( KIComparator, KIHash );
void KeyInfo_free(KeyInfo);

/* Accessors */
KIComparator KeyInfo_comparator(KeyInfo);
KIHash KeyInfo_hash(KeyInfo);


// ----------------------------------------------
// Common comparators and hash functions
// ----------------------------------------------

int KeyInfo_string_compare(const void* e1, const void* e2);
int KeyInfo_int_compare(const void* e1, const void* e2);
int KeyInfo_double_compare(const void* e1, const void* e2);
unsigned int KeyInfo_string_hash(const void* e1);
unsigned int KeyInfo_int_hash(const void* e);
unsigned int KeyInfo_double_hash(const void* e);
