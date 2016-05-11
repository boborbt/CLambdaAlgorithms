#pragma once

#include <stdlib.h>
#include "key_info.h"
#include "list.h"

// Opaque type definition
typedef struct _List* List;

// Constructor
List List_new(void);

// Given a pointer to a list, delete the head node from the pointed list and
// update the pointer to point to the new head.
void List_delete_node(List* list_ptr);

// Frees the list. If elem_free is not null each element is passed to
// elem_free so to allow user clean-up of the element before deallocing the node.
void List_free(List list, void (*elem_free)(void*));

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

// Returns a pointer to the sublist whose head satisfies elem_selector(elem)==1.
// Notice that this method requires a pointer to a list and returns a pointer
// to the sublist, while most of the other methods require and return lists
// (not pointers to them). This is necessary to allow the returned element to
// be used to delete list nodes.
List* List_find(List* list_ptr, int (*elem_selector)(const void*));

// Identical to List_find, but accept a block instead of a function.
List* List_find_wb(List* list_ptr, int (^elem_comparator)(const void*));
