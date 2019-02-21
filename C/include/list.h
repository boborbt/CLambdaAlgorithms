#pragma once

#include <stdlib.h>
#include "keys.h"
#include "list.h"
#include "iterator.h"


// Opaque type definition
typedef struct _List List;
typedef struct _ListNode ListNode;

// Constructor
List* List_new(void);

// Frees the list. If elem_free is not null each element is passed to
// elem_free so to allow user clean-up of the element before deallocing the node.
void List_free(List* list, void (*elem_free)(void*));

// Returns the element at the head of the given list.
// Semantically equivalent to List_iterator_get(ListIterator_new(list))
void* List_get_head(List* list);

// Returns the length of the given list. This is a O(1) operation.
size_t List_size(List* list);

// Returns 1 iff the list is empty
int List_empty(List* list);

// Inserts a new element at the head of the list.
void List_insert(List* list, void* elem);

// Insert a new element at the end of the list.
void List_append(List* list, void* elem);

// Deletes the given ListNode* from the list
void List_delete_node(List* list, ListNode* node);

// Returns the list node at the head of the list
ListNode* List_head(List* list);

// Returns the list node at the end of the list
ListNode* List_tail(List* list);

// Returns the element pointed by the given ListNode
void* List_get(ListNode* node);

// Returns the successor of the given ListNode. Returns null if
// the given ListNode is the last in the list
ListNode* List_next(ListNode*);

// Returns the predecessor of the given ListNode. Returns null if
// the given ListNode is the first in the list
ListNode* List_prev(ListNode*);


// Returns a pointer to the node that satisfies elem_selector(ListNode_get(elem))==1.
ListNode* List_find(List* list, int (*elem_selector)(const void*));

// Identical to List_find, but accept a block instead of a function.
ListNode* List_find_wb(List* list, int (^elem_comparator)(const void*));

// Returns the element stored in the given ListNode
void* ListNode_get(ListNode*);

//
// Iterators
//
typedef struct _ListIterator ListIterator;

// Creates a new list iterator over the given list. If list is NULL, NULL is
// returned. Note that ListIterator_end(NULL) returns true, all other Iterators
// functions, instead will produce undefined result on NULL values.
ListIterator* ListIterator_new(List* list);

// Creates a new list iterator from the given node. If node is NULL, NULL is
// returned. Note that ListIterator_end(NULL) returns true and ListIterator_free(NULL),
// will do nothing, all other iterators functions, instead, will produce undefined
// result when called on NULL values.
ListIterator* ListIterator_new_from_node(ListNode*);

// Frees the given iterator. Does nothing if the iterator is NULL
void ListIterator_free(ListIterator*);

// Returns the object pointed by the given iterator
void* ListIterator_get(ListIterator*);

// Returns the node pointed by the given iterator
ListNode* ListIterator_get_node(ListIterator*);

// Moves the iterator on the next element
void ListIterator_next(ListIterator*);

// Moves the iterator on the previous element
void ListIterator_prev(ListIterator* it);

// Moves the iterator to the end of the list
void ListIterator_to_end(ListIterator* it);

// Returns 1 if it1 and it2 points to the same element of the list or 0 otherwise.
int ListIterator_same(ListIterator* it1, ListIterator* it2);

// Returns true if either the iterator is null or if it moved after the last
// element
int ListIterator_end(ListIterator*);

// Store obj into the element currently pointed by the iterator
void ListIterator_set(ListIterator* it, void* obj);

// Iterator interface
// It returns a bidirectional mutable iterator
Iterator List_it(List* list);
