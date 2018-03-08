#include "dictionary.h"
#include "string.h"

Iterator Dictionary_it(Dictionary* dictionary) {
 return Iterator_make(
   dictionary,
   (void* (*)(void*)) DictionaryIterator_new,
   (void  (*)(void*))  DictionaryIterator_next,
   (void* (*)(void*)) DictionaryIterator_get,
   (int   (*)(void*))   DictionaryIterator_end,
   (int   (*)(void*, void*)) DictionaryIterator_same,
   (void  (*)(void*))  DictionaryIterator_free
 );
}


int Dictionary_empty(Dictionary* dictionary) {
  return Dictionary_size(dictionary) == 0;
}
