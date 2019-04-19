#include <stdlib.h>
#include "keys.h"
#include "list.h"
#include "errors.h"
#include <assert.h>
#include "array.h"

#include "mem.h"
#include "macros.h"
#include "iterator_functions.h"


// This definition only serves for silencing the compiler. The definition
// will not actually being used since the long ints will be used instead of ListNode pointers.
//
// In particular the ListNode for element at index n in the array will be defined to be the n+1
// long int cast to ListNode*. In this way the value 0 is compatible with the NULL value used
// to check if the ListNode is valid.
struct _ListNode {
  int _;
};

struct _List {
  Array* array;
};

struct _ListIterator {
  ArrayIterator* it;
};


/* --------------------------
 * List* implementation
 * -------------------------- */

List* List_new() {
  assert(sizeof(size_t) == sizeof(ListNode*));

  List* result = Mem_alloc(sizeof(struct _List));
  result->array = Array_new(10);
  return result;
}


void* List_get_head(List* list) {
  return Array_at(list->array, 0);
}

size_t List_size(List* list) {
  return Array_size(list->array);
}

int List_empty(List* list) {
  return Array_empty(list->array);
}

void List_insert(List* list, void* elem) {
  Array_insert(list->array, 0, elem);
}

void List_append(List* list, void* elem) {
  Array_add(list->array, elem);
}

ListNode* List_head(List* list) {
  if(Array_size(list->array) == 0) {
    return (ListNode*) 0;
  }

  return (ListNode*) 1;
}

ListNode* List_tail(List* list) {
  size_t result = Array_size(list->array);
  return (ListNode*) result;
}


void ListNode_set(List* list, ListNode* node, void* elem) {
  size_t index = (size_t)node - 1;
  Array_set(list->array, index, elem);
}

ListNode *List_next(List* list, ListNode * node) {
  size_t index = (size_t) node - 1;
  if( index + 1 >= Array_size(list->array)  ) {
    return NULL;
  }

  return (ListNode*)(index + 1 + 1);
}

ListNode *List_prev(UNUSED(List* list), ListNode * node) {
  if(node == NULL) {
    return NULL;
  }

  return (ListNode*)((size_t)node - 1);
}

void List_free(List* list, void (*elem_free)(void*)) {
  if(elem_free != NULL) {
    for_each( Array_it(list->array), ^(void* elem) {
      elem_free(elem);
    });
  }

  Array_free(list->array);
  Mem_free(list);
}

ListNode* List_find_wb(List* list,  int (^elem_selector)(const void*)) {
  __block size_t index = 0;
  void* result = find_first(Array_it(list->array), ^(void* elem) {
    index += 1;
    return elem_selector(elem) == 0;
  });

  if(result != NULL) {
    return (ListNode*) index;
  } else return NULL;
}

ListNode* List_find(List* list, int (*elem_selector)(const void*)) {
  return List_find_wb(list, ^int(const void* elem) {
      return elem_selector(elem);
  });
}

void* ListNode_get(List* list, ListNode* node) {
  size_t index = (size_t) node - 1;
  return Array_at(list->array, index);
}


void List_delete_node(List* list, ListNode* node) {
  size_t index = (size_t) node - 1;
  Array_remove(list->array, index);
}


ListIterator* ListIterator_new(List* list) {
  if(list == NULL) {
    return NULL;
  }

  ListIterator* it = Mem_alloc(sizeof(ListIterator));
  it->it = ArrayIterator_new(list->array);
  return it;
}

ListIterator* ListIterator_new_from_node(List* list, ListNode* node) {
  ListIterator* it = ListIterator_new(list);
  ArrayIterator_move_to(it->it, (size_t) node - 1);
  return it;
}

void ListIterator_free(ListIterator* it) {
  if(it!=NULL) {
    ArrayIterator_free(it->it);
    Mem_free(it);
  }
}

void* ListIterator_get(ListIterator* it) {
  return ArrayIterator_get(it->it);
}

void ListIterator_next(ListIterator* it) {
  ArrayIterator_next(it->it);
}

void ListIterator_prev(ListIterator* it) {
  ArrayIterator_prev(it->it);
}

void ListIterator_to_begin(ListIterator *it) { 
  ArrayIterator_to_begin(it->it); 
}

void ListIterator_to_end(ListIterator* it) {
  ArrayIterator_to_end(it->it);
}

int ListIterator_end(ListIterator* it) {
  if(it == NULL) {
    return 1;
  }
  return ArrayIterator_end(it->it);
}

int ListIterator_same(ListIterator* it1, ListIterator* it2) {
  return ArrayIterator_same(it1->it, it2->it);
}

void ListIterator_set(ListIterator* it, void* obj) {
  ArrayIterator_set(it->it, obj);
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
    (void  (*)(void*)) ListIterator_to_begin,
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
