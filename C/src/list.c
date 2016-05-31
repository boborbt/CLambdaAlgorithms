#include <stdlib.h>
#include "keys.h"
#include "list.h"
#include "errors.h"
#include <assert.h>


struct _ListNode {
  void* elem;
  struct _ListNode* pred;
  struct _ListNode* succ;
};

// Internal list implementation. It could be abstracted and moved into
// a separate module (not done since currently used only in this module).
struct _List {
  ListNode head;
  ListNode tail;
  size_t size;
};

struct _ListIterator {
  ListNode current;
};


/* --------------------------
 * List implementation
 * -------------------------- */

List List_new() {
  List result = (List) malloc(sizeof(struct _List));
  result->head = NULL;
  result->tail = NULL;
  result->size = 0l;

  return result;
}


void* List_get_head(List list) {
  return list->head->elem;
}

size_t List_length(List list) {
  return list->size;
}

int List_empty(List list) {
  return list->head == NULL;
}

// static void list_check(List list) {
//   ListNode last = NULL;
//   ListNode cur = list->head;
//   size_t count = 0;
//   while(cur != NULL) {
//     last = cur;
//     cur = cur->succ;
//     count += 1;
//   }
//
//   assert(count == list->size);
//   assert( last == list->tail );
//
//   cur = list->tail;
//   count = 0;
//   while(cur != NULL) {
//     last = cur;
//     cur = cur->pred;
//     count += 1;
//   }
//
//   assert(count == list->size);
//   assert( last == list->head );
// }

void List_insert(List list, void* elem) {
  ListNode new_node = (ListNode) malloc(sizeof(struct _ListNode));
  new_node->elem = elem;
  new_node->succ = list->head;
  new_node->pred = NULL;
  list->head = new_node;
  if(list->tail==NULL) {
    list->tail = new_node;
  }

  if(new_node->succ) {
    new_node->succ->pred = new_node;
  }

  list->size += 1;
}

void List_append(List list, void* elem) {
  ListNode new_node = (ListNode) malloc(sizeof(struct _ListNode));
  new_node->elem = elem;
  new_node->succ = NULL;
  new_node->pred = list->tail;
  list->tail = new_node;
  if(list->head==NULL) {
    list->head = new_node;
  }

  if(new_node->pred) {
    new_node->pred->succ = new_node;
  }

  list->size += 1;
}

ListNode List_head(List list) {
  return list->head;
}

ListNode List_tail(List list) {
  return list->tail;
}

void List_free(List list, void (*elem_free)(void*)) {
  ListNode current = list->head;
  while(current!=NULL) {
    ListNode next = current->succ;
    if(elem_free) {
      elem_free(current->elem);
    }
    free(current);
    current = next;
  }

  free(list);
}

ListNode List_find_wb(List list,  int (^elem_selector)(const void*)) {
  ListIterator it = ListIterator_new(list);

  while( !ListIterator_end(it) && elem_selector(ListIterator_get(it)) != 0 ) {
    ListIterator_next(it);
  }

  if(!ListIterator_end(it)) {
    return ListIterator_get_node(it);
  } else {
    return NULL;
  }
}

ListNode List_find(List list, int (*elem_selector)(const void*)) {
  return List_find_wb(list, ^int(const void* elem) {
      return elem_selector(elem);
  });
}

void* ListNode_get(ListNode node) {
  return node->elem;
}


void List_delete_node(List list, ListNode node) {
  if(list->size == 0) {
    Error_raise( Error_new( ERROR_GENERIC, "Trying to delete from an empty list" ) );
  }

  if(list->head == node) {
    list->head = node->succ;
  }

  if(list->tail == node) {
    list->tail = node->pred;
  }

  if(node->succ != NULL) {
    node->succ->pred = node->pred;
  }

  if(node->pred != NULL) {
    node->pred->succ = node->succ;
  }

  free(node);

  list->size -= 1;
}


ListIterator ListIterator_new(List list) {
  if(list == NULL) {
    return NULL;
  }

  ListIterator result = (ListIterator) malloc(sizeof(struct _ListIterator));
  result->current = list->head;
  return result;
}

ListIterator ListIterator_new_from_node(ListNode node) {
  if(node == NULL) {
    return NULL;
  }

  ListIterator result = (ListIterator) malloc(sizeof(struct _ListIterator));
  result->current = node;
  return result;
}

void ListIterator_free(ListIterator it) {
  if(it!=NULL) {
    free(it);
  }
}

void* ListIterator_get(ListIterator it) {
  return it->current->elem;
}

ListNode ListIterator_get_node(ListIterator it) {
  return it->current;
}

void ListIterator_next(ListIterator it) {
  it->current = it->current->succ;
}

int ListIterator_end(ListIterator it) {
  return it==NULL || it->current == NULL;
}

void foreach_list_element(List list, void(^callback)(void*)) {
  ListIterator it = ListIterator_new(list);
  while( !ListIterator_end(it) ) {
    callback(ListIterator_get(it));
    ListIterator_next(it);
  }
  ListIterator_free(it);
}
