#include "dictionary.h"
#include "string.h"

// Returns the key of the element currently pointed by the iterator
void* DictionaryIterator_key_get(DictionaryIterator* it) {
  return DictionaryIterator_get(it)->key;
}

Iterator Dictionary_it(Dictionary* dictionary) {
 return Iterator_make(
   dictionary,
   (void* (*)(void*)) DictionaryIterator_new,
   (void  (*)(void*))  DictionaryIterator_next,
   (void* (*)(void*)) DictionaryIterator_get,
   (int   (*)(void*))   DictionaryIterator_end,
   (void  (*)(void*)) DictionaryIterator_to_begin,
   (int   (*)(void*, void*)) DictionaryIterator_same,
   (void  (*)(void*))  DictionaryIterator_free
 );
}

Iterator Dictionary_key_it(Dictionary* dictionary) {
  return Iterator_make(
    dictionary,
    (void* (*)(void*)) DictionaryIterator_new,
    (void  (*)(void*))  DictionaryIterator_next,
    (void* (*)(void*)) DictionaryIterator_key_get,
    (int   (*)(void*))   DictionaryIterator_end,
    (void  (*)(void*)) DictionaryIterator_to_begin,
    (int   (*)(void*, void*)) DictionaryIterator_same,
    (void  (*)(void*))  DictionaryIterator_free
  );
}



int Dictionary_empty(Dictionary* dictionary) {
  return Dictionary_size(dictionary) == 0;
}
