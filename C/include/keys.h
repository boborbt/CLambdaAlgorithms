#pragma once

#include <stdlib.h>

/*
 * KeyInfo* maintains a pair of functions to compare and
 * create hashes of keys.
 */

typedef struct _KeyInfo KeyInfo;
typedef int (*KIComparator)(const void*, const void*);
typedef size_t (*KIHash)(const void*);

/*
 * Definition of the key/value pair type. It is used in all dictionaries
 * to store the key/value association.
 */

typedef struct _KeyValue {
  void* key;
  void* value;
} KeyValue;


//
// KeyInfo* definitions
//

/* Constructor and destructor*/
KeyInfo* KeyInfo_new( KIComparator, KIHash );
void KeyInfo_free(KeyInfo*);

/* Accessors */
KIComparator KeyInfo_comparator(KeyInfo*);
KIHash KeyInfo_hash(KeyInfo*);


// ----------------------------------------------
// Common comparators and hash functions
// ----------------------------------------------

int Key_string_compare(const void* e1, const void* e2);
int Key_int_compare(const void* e1, const void* e2);
int Key_long_compare(const void* e1, const void* e2);
int Key_double_compare(const void* e1, const void* e2);
size_t Key_string_hash(const void* e1);
size_t Key_int_hash(const void* e);
size_t Key_double_hash(const void* e);
