#include <stdlib.h>
#include "key_info.h"
#include "list.h"


// Internal list implementation. It could be abstracted and moved into
// a separate module (not done since currently used only in this module).
struct _List {
  void* elem;
  struct _List* next;
};


/* --------------------------
 * List implementation
 * -------------------------- */

List List_new() {
  return NULL;
}


List List_next(List list) {
  return list->next;
}

void* List_get(List list) {
  return list->elem;
}

unsigned int List_length(List list) {
  unsigned int count = 0;
  while(list != NULL) {
    count += 1;
    list = list->next;
  }

  return count;
}

List List_insert(List list, void* elem) {
  List new_node = (List) malloc(sizeof(struct _List));
  new_node->elem = elem;
  new_node->next = list;

  return new_node;
}

void List_free(List list, void (*elem_free)(void*)) {
  while(list!=NULL) {
    List tmp = list->next;
    if(elem_free) {
      elem_free(list->elem);
    }
    free(list);
    list = tmp;
  }
}



void* List_find_wb(List list, const void* key, int (^elem_comparator)(const void*, const void*)) {
  while(list != NULL && elem_comparator(key, list->elem) != 0) {
    list = list->next;
  }

  if(list==NULL) {
    return NULL;
  }

  return list->elem;
}



void* List_find(List list, const void* key, int (*elem_comparator)(const void*, const void*)) {
  return List_find_wb(list, key, ^ int (const void* key1, const void* key2){
    return elem_comparator(key1, key2);
  });
}





List* List_find_node_wb(List* list_ptr, const void* key, int (^elem_comparator)(const void*, const void*)) {
  if(*list_ptr == NULL) {
    return NULL;
  }

  while( (*list_ptr) != NULL && elem_comparator(key, (*list_ptr)->elem) != 0 ) {
    list_ptr = &(*list_ptr)->next;
  }

  return list_ptr;
}

List* List_find_node(List* list_ptr, const void* key, int (*elem_comparator)(const void*, const void*)) {
  return List_find_node_wb(list_ptr, key, ^int(const void* key1, const void* key2) {
      return elem_comparator(key1, key2);
    });
}


void List_delete_node(List* list_ptr) {
  List tmp = *list_ptr;
  *list_ptr = (*list_ptr)->next;
  free(tmp);
}
