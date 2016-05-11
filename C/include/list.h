#pragma once

#include <stdlib.h>
#include "key_info.h"
#include "list.h"

// Opaque type definition
typedef struct _List* List;

// Constructor and destructors
List List_new(void);
void List_delete_node(List* list_ptr);
void List_free(List list, void (*Elem_free)(void*));

// Iterator
List List_next(List list);


// Returns the element at the head of the given list
void* List_get(List list);

// Returns the length of the given list. This is a O(n) operation.
unsigned int List_length(List list);

// Inserts a new element in the list and returns a pointer to the new head
// intended usage is something like:
// list = List_insert(list, new_elem);
List List_insert(List list, void* elem);

// // Finds an element based on the given key and a comparator function. The function
// // returns the element in the list such that elem_conparator(key, elem) == 0.
// void* List_find(List list, const void* key, int (*elem_comparator)(const void*, const void*));
//
// // Finds with block. Finds an element based on the given key and a comparator
// // lambda. The function returns the element in the list such that
// // elem_conparator(key, elem) == 0.
// void* List_find_wb(List list, const void* key, int (^elem_comparator)(const void*, const void*));

// Finds the node of the list containing an element having the given key.
// It's identical to List_find with the exception that the whole list node is
// returned (instead of the element contained in the node)
List* List_find(List* list_ptr, int (*elem_selector)(const void*));

List* List_find_wb(List* list_ptr, int (^elem_comparator)(const void*));
