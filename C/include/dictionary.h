#pragma once

#include "keys.h"
#include "iterator.h"

#include <string.h>

typedef struct _Dictionary* Dictionary;

/*
 * Constructor and destructor
 */
Dictionary Dictionary_new(KeyInfo keyInfo);
void Dictionary_free(Dictionary dictionary);

// insert into dictionary the given key/value pair. If key is already
// present, the dictionary is updated with the new value. Otherwise, the
// key/value pair is inserted.
void Dictionary_set(Dictionary dictionary, void* key, void* value);

// Retrieve the value associated with the given key. The found value
// is put into *result unless result==NULL.
// If the key is not found, the function returns 0 and leave result untouched.
// Otherwise it returns 1 and, if result!=NULL, sets *results to point to the found KeyValue.
int Dictionary_get(Dictionary dictionary, const void* key, void** result);

// Delete key from the given dictionary. Do nothing if key does not belong
// to the dictionary.
void Dictionary_delete(Dictionary dictionary, const void* key);

// Returns the number of distinct keys stores in the dictionary.
size_t Dictionary_size(Dictionary dictionary);
int Dictionary_empty(Dictionary dictionary);

// Returns an evaluation of the efficiency of the dictionary. Its actual
// value is implementation specific, but it should be lower for dictionaries
// working in optimal settings, and higher for degenerate situations.
double Dictionary_efficiency_score(Dictionary);

// Returns 1 if the dictionary integrity is ok. Return 0 otherwise.
int Dictionary_check_integrity(Dictionary);

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
KeyValue* DictionaryIterator_get(DictionaryIterator it);


// Important notice: the iterator will return KeyValue* objects.
// so a tipical for_each will be something like:
// for_each(Dictionary_it(dictionary), ^(void* obj) {
//    KeyValue* kv = (KeyValue*) obj;
//    ... do your work ...
// });
Iterator Dictionary_it(Dictionary);
