#ifndef __DICTIONARY_H_3NXN3XL378__
#define __DICTIONARY_H_3NXN3XL378__

#include "key_info.h"
#include <string.h>

typedef struct _Dictionary* Dictionary;

typedef struct _Elem {
  const void* key;
  const void* value;
}* Elem;

/*
 * Constructor and destructor
 */
Dictionary Dictionary_new(KeyInfo keyInfo);
void Dictionary_free(Dictionary dictionary);

// insert into dictionary the given key/value pair. If key is already
// present, the dictionary is updated with the new value. Otherwise, the
// key/value pair is inserted.
void Dictionary_set(Dictionary dictionary, const void* key, const void* value);

// Retrieve the value associated with the given key. The found value
// is put into *result.
// If the key is not found, the function returns 0 and leave result untouched.
// Otherwise it returns 1.
int Dictionary_get(Dictionary dictionary, const void* key, const void** result);

// Delete key from the given dictionary. Do nothing if key does not belong
// to the dictionary.
void Dictionary_delete(Dictionary dictionary, const void* key);

// Returns the number of distinct keys stores in the dictionary.
unsigned int Dictionary_size(Dictionary dictionary);
int Dictionary_empty(Dictionary dictionary);

// Returns an evaluation of the efficiency of the dictionary. Its actual
// value is implementation specific, but it should be lower for dictionaries
// working in optimal settings, and higher for degenerate situations.
double Dictionary_efficiency_score();

// -------------------------------------
// Iterator interface to the dictionary
// -------------------------------------

typedef struct _DictionaryIterator* DictionaryIterator;

// constructor and destructor
DictionaryIterator DictionaryIterator_new(Dictionary dictionary);
void DictionaryIterator_free(DictionaryIterator it);

// Move the iterator to the next element. Do nothing if it is already past the
// end of the container.
void DictionaryIterator_next(DictionaryIterator it);

// Returns 1 if the iterator is past the end of the container (i.e., if
// DictionaryIterator_get would return a sensible result), 0 otherwise.
int DictionaryIterator_end(DictionaryIterator it);

// Returns the element currently pointed by the iterator
Elem DictionaryIterator_get(DictionaryIterator it);


#endif
