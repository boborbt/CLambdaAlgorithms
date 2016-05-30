#include "dictionary.h"
#include <stdlib.h>
#include "list.h"

#define HASH_TABLE_INITIAL_CAPACITY 4096
#define HASH_TABLE_CAPACITY_MIN_MULTIPLIER 2
#define HASH_TABLE_CAPACITY_MAX_MULTIPLIER 4
#define HASH_TABLE_CAPACITY_MULTIPLIER 2

/* --------------------------
 * Opaque structures definitions
 * -------------------------- */
struct _Dictionary {
  List* table;
  size_t capacity;
  size_t size;
  KeyInfo keyInfo;
};

struct _DictionaryIterator {
  Dictionary dictionary;
  size_t cur_index;
  List cur_list_element;
};

/* KeyValue* constructor and destructor */

static KeyValue* KeyValue_new(void* key, void* value) {
  KeyValue* result = (KeyValue*) malloc(sizeof(struct _KeyValue*));
  result->key = key;
  result->value = value;
  return result;
}

static const void* KeyValue_key(const KeyValue*  elem) {
  return elem->key;
}

static void KeyValue_free(KeyValue* kv) {
  free(kv);
}

/* --------------------------
 * DictionaryIterator implementation
 *  -------------------------- */

DictionaryIterator DictionaryIterator_new(Dictionary dictionary) {
  DictionaryIterator it = (DictionaryIterator) malloc(sizeof(struct _DictionaryIterator));
  it->dictionary = dictionary;
  it->cur_index = 0;
  it->cur_list_element = it->dictionary->table[0];
  if(it->cur_list_element == NULL) {
    DictionaryIterator_next(it);
  }

  return it;
}

void DictionaryIterator_free(DictionaryIterator it) {
  free(it);
}

int DictionaryIterator_end(DictionaryIterator it)  {
  return it->cur_list_element == NULL;
}

KeyValue* DictionaryIterator_get(DictionaryIterator it) {
  return List_get(it->cur_list_element);
}

void DictionaryIterator_next(DictionaryIterator it) {
  if(it->cur_list_element != NULL) {
    it->cur_list_element = List_next(it->cur_list_element);
    if(it->cur_list_element != NULL) {
        return;
    }
  }

  while(it->cur_index < it->dictionary->capacity - 1 && it->cur_list_element == NULL) {
    it->cur_index += 1;
    it->cur_list_element = it->dictionary->table[it->cur_index];
  }
}



/* --------------------------
 * Dictionary implementation
 * -------------------------- */

static void Dictionary_free_lists(Dictionary dictionary, void (*elem_free)(void*)) {
  size_t capacity = dictionary->capacity;
  for(size_t i=0; i<capacity; ++i) {
    if(dictionary->table[i]!=NULL) {
      List_free(dictionary->table[i], elem_free);
    }
  }
}

Dictionary Dictionary_new(KeyInfo keyInfo) {
  Dictionary result = (Dictionary) malloc( sizeof(struct _Dictionary) );
  result->table = (List*) calloc(HASH_TABLE_INITIAL_CAPACITY, sizeof(List));
  result->capacity = HASH_TABLE_INITIAL_CAPACITY;
  result->size = 0;
  result->keyInfo = keyInfo;

  return result;
}

void Dictionary_free(Dictionary dictionary) {
  Dictionary_free_lists(dictionary, (void (*)(void*)) KeyValue_free);
  free(dictionary->table);
  free(dictionary);
}

// Reallocate the items in the current dictionary doubling the number of
// buckets. This is a costly operation since all bucket needs to be relocated
// to new places.
static void Dictionary_realloc(Dictionary dictionary, size_t new_capacity) {
  List* new_table = (List*) calloc(new_capacity, sizeof(List));

  DictionaryIterator it = DictionaryIterator_new(dictionary);
  while(!DictionaryIterator_end(it)) {
    KeyValue* current = DictionaryIterator_get(it);
    size_t index = KeyInfo_hash(dictionary->keyInfo)(current->key) % new_capacity;
    new_table[index] = List_insert(new_table[index], current);

    DictionaryIterator_next(it);
  };

  DictionaryIterator_free(it);

  Dictionary_free_lists(dictionary, 0);
  free(dictionary->table);
  dictionary->table = new_table;
  dictionary->capacity = new_capacity;
}

// insert into dictionary the given key/value pair. If key is already
// present, the dictionary is updated with the new value. Otherwise, the
// key/value pair is inserted.
void Dictionary_set(Dictionary dictionary, void* key, void* value) {
  if(dictionary->capacity < dictionary->size * HASH_TABLE_CAPACITY_MIN_MULTIPLIER) {
    Dictionary_realloc(dictionary, dictionary->capacity * HASH_TABLE_CAPACITY_MULTIPLIER);
  }

  size_t index = KeyInfo_hash(dictionary->keyInfo)(key) % dictionary->capacity;

  List* list_elem = List_find_wb(&dictionary->table[index], ^int (const void* elem) {
      return KeyInfo_comparator(dictionary->keyInfo)(key, KeyValue_key((const KeyValue*) elem));
    });

  if(*list_elem != NULL) {
    KeyValue* kv = List_get(*list_elem);
    kv->key = key;
    kv->value = value;
    return;
  }

  dictionary->table[index] = List_insert(dictionary->table[index], KeyValue_new(key, value));
  dictionary->size += 1;
}


static KeyValue* Dictionary_get_key_value(Dictionary dictionary, const void* key) {
  size_t index = KeyInfo_hash(dictionary->keyInfo)(key) % dictionary->capacity;
  if(dictionary->table[index] == NULL) {
    return 0;
  }

  List* list_elem = List_find_wb(&dictionary->table[index], ^int(const void* elem) {
      return KeyInfo_comparator(dictionary->keyInfo)(key, KeyValue_key((const KeyValue*) elem));
    });

  if(*list_elem == NULL) {
    return NULL;
  } else {
    return List_get(*list_elem);
  }
}


// Retrieve the value associated with the given key. The found value
// is put into *result unless result==NULL.
// If the key is not found, the function returns 0 and leave result untouched.
// Otherwise it returns 1 and, if result!=NULL, sets *results to point to the found KeyValue.
int Dictionary_get(Dictionary dictionary, const void* key,  void** result) {
  KeyValue* kv = Dictionary_get_key_value(dictionary, key);

  if(kv == NULL) {
    return 0;
  }

  if(result!=NULL) {
    *result = (void*) kv->value;
  }

  return 1;
}


void Dictionary_delete(Dictionary dictionary, const void* key) {
  size_t index = KeyInfo_hash(dictionary->keyInfo)(key) % dictionary->capacity;
  if(dictionary->table[index] == NULL) {
    return;
  }

  List* list_ptr = List_find_wb(&dictionary->table[index], ^int (const void* elem) {
      return KeyInfo_comparator(dictionary->keyInfo)(key, KeyValue_key((const KeyValue*) elem));
  });

  if(*list_ptr == NULL) {
    return;
  }

  dictionary->size -= 1;
  List_delete_node(list_ptr);

  if(dictionary->capacity > HASH_TABLE_INITIAL_CAPACITY &&
     dictionary->capacity > dictionary->size * HASH_TABLE_CAPACITY_MAX_MULTIPLIER ) {
    Dictionary_realloc(dictionary, dictionary->capacity / HASH_TABLE_CAPACITY_MULTIPLIER);
  }
}

size_t Dictionary_size(Dictionary dictionary) {
  return dictionary->size;
}

double Dictionary_efficiency_score(Dictionary dictionary) {
  size_t sum_len = 0;
  size_t buckets_count = 0;
  for(size_t i=0; i<dictionary->capacity; ++i) {
    if(dictionary->table[i] != NULL) {
      sum_len += List_length(dictionary->table[i]);
      buckets_count += 1;
    }
  }

  return (double) sum_len / buckets_count;
}

int Dictionary_check_integrity(Dictionary dictionary) {
  // to be implemented. The following test is just to avoid param unused warning.
  return dictionary->table != NULL;
}
