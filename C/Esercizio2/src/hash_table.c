#include "dictionary.h"
#include <stdlib.h>

#define HASH_TABLE_INITIAL_CAPACITY 4096

typedef struct _List {
  Elem elem;
  struct _List* next;
}* List;

struct _Dictionary {
  List* table;
  unsigned int capacity;
  unsigned int size;
  KeyInfo keyInfo;
};

struct _DictionaryIterator {
  Dictionary dictionary;
  int cur_index;
  List cur_list_element;
};


Elem Elem_new(void* key, void* value) {
  Elem result = (Elem) malloc(sizeof(struct _Elem));
  result->key = key;
  result->value = value;
  return result;
}

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

Elem DictionaryIterator_get(DictionaryIterator it) {
  return it->cur_list_element->elem;
}

void DictionaryIterator_next(DictionaryIterator it) {
  it->cur_list_element = it->cur_list_element->next;
  if(it->cur_list_element != NULL) {
      return;
  }

  while(it->cur_index < it->dictionary->capacity - 1 && it->cur_list_element == NULL) {
    it->cur_index += 1;
    it->cur_list_element = it->dictionary->table[it->cur_index];
  }
}

unsigned int List_length(List list) {
  int count = 0;
  while(list != NULL) {
    count += 1;
    list = list->next;
  }

  return count;
}

List List_insert(List list, Elem elem) {
  List new_node = (List) malloc(sizeof(struct _List));
  new_node->elem = elem;
  new_node->next = list;

  return new_node;
}

void List_free(List list, int free_elems) {
  while(list!=NULL) {
    List tmp = list->next;
    if(free_elems) {
      free(list->elem);
    }
    free(list);
    list = tmp;
  }
}

Elem List_find(List list, void* key, KeyInfo keyInfo) {
  while(list != NULL && KeyInfo_comparator(keyInfo)(key, list->elem->key) != 0) {
    list = list->next;
  }

  if(list==NULL) {
    return NULL;
  }

  return list->elem;
}

List* List_find_node(List* list_ptr, void* key, KeyInfo keyInfo) {
  if(*list_ptr == NULL) {
    return NULL;
  }

  while( (*list_ptr) != NULL && KeyInfo_comparator(keyInfo)(key, (*list_ptr)->elem->key) != 0 ) {
    list_ptr = &(*list_ptr)->next;
  }

  return list_ptr;
}


void Dictionary_free_lists(Dictionary dictionary, int free_elems) {
  unsigned int capacity = dictionary->capacity;
  for(int i=0; i<capacity; ++i) {
    if(dictionary->table[i]!=NULL) {
      List_free(dictionary->table[i], free_elems);
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
  Dictionary_free_lists(dictionary, 1);
  free(dictionary->table);
  free(dictionary);
}

void Dictionary_realloc(Dictionary dictionary) {
  unsigned int new_capacity = dictionary->capacity * 2;
  List* new_table = (List*) calloc(new_capacity, sizeof(List));

  DictionaryIterator it = DictionaryIterator_new(dictionary);
  while(!DictionaryIterator_end(it)) {
    Elem current = DictionaryIterator_get(it);
    int index = KeyInfo_hash(dictionary->keyInfo)(current->key) % new_capacity;
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
  if(dictionary->capacity < dictionary->size / 2) {
    Dictionary_realloc(dictionary);
  }

  unsigned int index = KeyInfo_hash(dictionary->keyInfo)(key) % dictionary->capacity;

  Elem elem = List_find(dictionary->table[index], key, dictionary->keyInfo );
  if(elem != NULL) {
    elem->key = key;
    elem->value = value;
    return;
  }

  dictionary->table[index] = List_insert(dictionary->table[index], Elem_new(key, value));
  dictionary->size += 1;
}

// Retrieve the value associated with the given key. The found value
// is put into *result.
// If the key is not found, the function returns 0 and leave result untouched.
// Otherwise it returns 1.
int Dictionary_get(Dictionary dictionary, void* key, void** result) {
  unsigned int index = KeyInfo_hash(dictionary->keyInfo)(key) % dictionary->capacity;
  if(dictionary->table[index] == NULL) {
    return 0;
  }

  Elem elem = List_find(dictionary->table[index], key, dictionary->keyInfo );
  if(elem == NULL) {
    return 0;
  } else {
    *result = elem->value;
    return 1;
  }
}


void Dictionary_delete(Dictionary dictionary, void* key) {
  unsigned int index = KeyInfo_hash(dictionary->keyInfo)(key) % dictionary->capacity;
  if(dictionary->table[index] == NULL) {
    return;
  }

  List* list_ptr = List_find_node(&dictionary->table[index], key, dictionary->keyInfo);
  if(*list_ptr == NULL) {
    return;
  }

  dictionary->size -= 1;
  List tmp = *list_ptr;
  *list_ptr = (*list_ptr)->next;
  free(tmp);
}

unsigned int Dictionary_size(Dictionary dictionary) {
  return dictionary->size;
}

double Dictionary_efficiency_score(Dictionary dictionary) {
  unsigned int sum_len = 0;
  unsigned int buckets_count = 0;
  for(int i=0; i<dictionary->capacity; ++i) {
    if(dictionary->table[i] != NULL) {
      sum_len += List_length(dictionary->table[i]);
      buckets_count += 1;
    }
  }

  return (double) sum_len / buckets_count;
}
