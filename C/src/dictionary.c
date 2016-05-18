#include "dictionary.h"
#include "string.h"

int Dictionary_empty(Dictionary dictionary) {
  return Dictionary_size(dictionary) == 0;
}

void foreach_dictionary_key_value(Dictionary dictionary, void (^callback)(KeyValue* kv)) {
  DictionaryIterator it = DictionaryIterator_new(dictionary);
  while(!DictionaryIterator_end(it)) {
    callback(DictionaryIterator_get(it));
    DictionaryIterator_next(it);
  }
  DictionaryIterator_free(it);
}
