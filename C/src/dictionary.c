#include "dictionary.h"
#include "string.h"

Iterator Dictionary_it(Dictionary dictionary) {
 return Iterator_make(
   dictionary,
   (void* (*)(void*)) DictionaryIterator_new,
   (void (*)(void*))  DictionaryIterator_next,
   (void* (*)(void*)) DictionaryIterator_get,
   (int (*)(void*))   DictionaryIterator_end,
   (void (*)(void*))  DictionaryIterator_free
 );
}


int Dictionary_empty(Dictionary dictionary) {
  return Dictionary_size(dictionary) == 0;
}

KeyValue* find_dictionary_key_value(Dictionary dictionary, int (^callback)(KeyValue* kv)) {
  DictionaryIterator it = DictionaryIterator_new(dictionary);
  int found = 0;
  KeyValue* result = NULL;
  while(!DictionaryIterator_end(it) && !found) {
    result = DictionaryIterator_get(it);
    found = callback(result);
    DictionaryIterator_next(it);
  }
  DictionaryIterator_free(it);

  return found ? result : NULL ;
}
