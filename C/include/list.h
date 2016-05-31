#pragma once

#include <stdlib.h>
#include "keys.h"
#include "list.h"

// Opaque type definition
typedef struct _List* List;
typedef struct _ListNode* ListNode;

// Constructor
List List_new(void);

// Given a pointer to a list, delete the head node from the pointed list and
// update the pointer to point to the new head.
void List_delete_node(List list, ListNode node);

// Frees the list. If elem_free is not null each element is passed to
// elem_free so to allow user clean-up of the element before deallocing the node.
void List_free(List list, void (*elem_free)(void*));

// Returns the element at the head of the given list
void* List_get_head(List list);

// Returns the length of the given list. This is a O(n) operation.
size_t List_length(List list);

// Inserts a new element in the list and returns a pointer to the new head
// intended usage is something like:
// list = List_insert(list, new_elem);
void List_insert(List list, void* elem);

// Returns a pointer to the sublist whose head satisfies elem_selector(elem)==1.
// Notice that this method requires a pointer to a variable holding a list and
// returns a pointer to the variable in the list that points to the the sublist,
// while most of the other methods require and return lists
// (not pointers to them). This allows the returned pointer to be used to change
// the list itself. For instance if List* subl = List_find(...), then
// List_delete_node(subl) will remove the head of subl from the original list
// changing the original list head if necessary.
ListNode List_find(List list, int (*elem_selector)(const void*));

// Identical to List_find, but accept a block instead of a function.
ListNode List_find_wb(List list, int (^elem_comparator)(const void*));

void* ListNode_get(ListNode);

//
// Iterators
//
typedef struct _ListIterator* ListIterator;

// Creates a new list iterator over the given list. If list is NULL, NULL is
// returned. Note that ListIterator_end(NULL) returns true, all other Iterators
// functions, instead will produce undefined result on NULL values.
ListIterator ListIterator_new(List list);

// Creates a new list iterator from the given node. If node is NULL, NULL is
// returned. Note that ListIterator_end(NULL) returns true and ListIterator_free(NULL),
// will do nothing, all other iterators functions, instead, will produce undefined
// result when called on NULL values.
ListIterator ListIterator_new_from_node(ListNode);

// Frees the given iterator. Does nothing if the iterator is NULL
void ListIterator_free(ListIterator);

// Returns the object pointed by the given iterator
void* ListIterator_get(ListIterator);

// Returns the node pointed by the given iterator
ListNode ListIterator_get_node(ListIterator);

// Moves the iterator on the next element
void ListIterator_next(ListIterator);

// Returns true if either the iterator is null or if it moved after the last
// element
int ListIterator_end(ListIterator);

// foreach
void foreach_list_element(List list, void(^)(void*));
