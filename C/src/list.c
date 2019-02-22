#include <stdlib.h>
#include "keys.h"
#include "list.h"
#include "errors.h"
#include <assert.h>

#include "mem.h"
#include "macros.h"


struct _ListNode {
  void* elem;
  struct _ListNode* pred;
  struct _ListNode* succ;
};

// Internal list implementation. It could be abstracted and moved into
// a separate module (not done since currently used only in this module).
struct _List {
  ListNode* head;
  ListNode* tail;
  size_t size;
};

struct _ListIterator {
  ListNode* current;
  ListNode* tail;
};


/* --------------------------
 * List* implementation
 * -------------------------- */

List* List_new() {
  List* result = (List*) Mem_alloc(sizeof(struct _List));
  result->head = NULL;
  result->tail = NULL;
  result->size = 0l;

  return result;
}


void* List_get_head(List* list) {
  return list->head->elem;
}

size_t List_size(List* list) {
  return list->size;
}

int List_empty(List* list) {
  return list->head == NULL;
}

// static void list_check(List* list) {
//   ListNode* last = NULL;
//   ListNode* cur = list->head;
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

void List_insert(List* list, void* elem) {
  ListNode* new_node = (ListNode*) Mem_alloc(sizeof(struct _ListNode));
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

void List_append(List* list, void* elem) {
  ListNode* new_node = (ListNode*) Mem_alloc(sizeof(struct _ListNode));
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

ListNode* List_head(List* list) {
  return list->head;
}

ListNode* List_tail(List* list) {
  return list->tail;
}


void ListNode_set(UNUSED(List* list), ListNode* node, void* elem) {
  node->elem = elem;
}

ListNode *List_next(UNUSED(List* list), ListNode * node) {
  return node->succ;
}

ListNode *List_prev(UNUSED(List* list), ListNode * node) {
  return node->pred;
}

void List_free(List* list, void (*elem_free)(void*)) {
  ListNode* current = list->head;
  while(current!=NULL) {
    ListNode* next = current->succ;
    if(elem_free) {
      elem_free(current->elem);
    }
    Mem_free(current);
    current = next;
  }

  Mem_free(list);
}

ListNode *ListIterator_get_node(ListIterator *it);

ListNode *List_find_wb(List *list, int (^elem_selector)(const void *)) {
  ListIterator* it = ListIterator_new(list);

  while( !ListIterator_end(it) && elem_selector(ListIterator_get(it)) != 0 ) {
    ListIterator_next(it);
  }

  if(!ListIterator_end(it)) {
    ListNode* result =  ListIterator_get_node(it);
    ListIterator_free(it);
    return result;
  } else {
    ListIterator_free(it);
    return NULL;
  }
}

ListNode* List_find(List* list, int (*elem_selector)(const void*)) {
  return List_find_wb(list, ^int(const void* elem) {
      return elem_selector(elem);
  });
}

void* ListNode_get(UNUSED(List* list), ListNode* node) {
  return node->elem;
}


void List_delete_node(List* list, ListNode* node) {
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

  Mem_free(node);

  list->size -= 1;
}


ListIterator* ListIterator_new(List* list) {
  if(list == NULL) {
    return NULL;
  }

  ListIterator* result = (ListIterator*) Mem_alloc(sizeof(struct _ListIterator));
  result->current = list->head;
  result->tail = list->tail;
  return result;
}

ListIterator* ListIterator_new_from_node(UNUSED(List* list), ListNode* node) {
  if(node == NULL) {
    return NULL;
  }

  ListIterator* result = (ListIterator*) Mem_alloc(sizeof(struct _ListIterator));
  result->current = node;
  return result;
}

void ListIterator_free(ListIterator* it) {
  if(it!=NULL) {
    Mem_free(it);
  }
}

void* ListIterator_get(ListIterator* it) {
  return it->current->elem;
}

ListNode* ListIterator_get_node(ListIterator* it) {
  return it->current;
}

void ListIterator_next(ListIterator* it) {
  it->current = it->current->succ;
}

void ListIterator_prev(ListIterator* it) {
  it->current = it->current->pred;
}

void ListIterator_to_end(ListIterator* it) {
  it->current = it->tail;
}

int ListIterator_end(ListIterator* it) {
  return it==NULL || it->current == NULL;
}

int ListIterator_same(ListIterator* it1, ListIterator* it2) {
  return  it1->current == it2->current &&
          it1->tail == it2->tail;
}

void ListIterator_set(ListIterator* it, void* obj) {
  it->current->elem = obj;
}

static void* ListIterator_alloc_obj(ListIterator* UNUSED(it)) {
  return NULL;
}

static void* ListIterator_copy_obj(ListIterator* it, void* UNUSED(to_mem)) {
  return ListIterator_get(it);
}

static void ListIterator_free_obj(UNUSED(void* obj)) {
  return;
}

Iterator List_it(List* list) {
  Iterator iterator = Iterator_make(
    list,
    (void* (*)(void*)) ListIterator_new,
    (void (*)(void*))  ListIterator_next,
    (void* (*)(void*)) ListIterator_get,
    (int (*)(void*))   ListIterator_end,
    (int (*)(void*, void*)) ListIterator_same,
    (void (*)(void*))  ListIterator_free
  );

  iterator = BidirectionalIterator_make(iterator,
    (void  (*)(void*)) ListIterator_prev,
    (void  (*)(void*)) ListIterator_to_end
  );

  iterator = MutableIterator_make(iterator,
    (void (*)(void*, void*)) ListIterator_set
  );

  iterator = CloningIterator_make(iterator,
    (void*(*)(void*)) ListIterator_alloc_obj,
    (void* (*)(void*, void*)) ListIterator_copy_obj,
    (void (*)(void*)) ListIterator_free_obj
  );

  return iterator;
}
