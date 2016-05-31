#include "dictionary.h"
#include "string.h"

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

void foreach_dictionary_key_value(Dictionary dictionary, void (^callback)(KeyValue* kv)) {
  find_dictionary_key_value(dictionary, ^(KeyValue* kv) {
    callback(kv);
    return 0;
  });
}
