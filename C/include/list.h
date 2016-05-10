#pragma once

#include <stdlib.h>
#include "key_info.h"
#include "list.h"

typedef struct _List* List;


/* --------------------------
 * List implementation
 * -------------------------- */

List List_new(void);
List List_next(List list);
void* List_get(List list);
unsigned int List_length(List list);
List List_insert(List list, void* elem);
void List_free(List list, void (*Elem_free)(void*));
void* List_find(List list, const void* key, int (*elem_comparator)(const void*, const void*));
void* List_find_wb(List list, const void* key, int (^elem_comparator)(const void*, const void*));


List* List_find_node(List* list_ptr, const void* key, int (*elem_comparator)(const void*, const void*));
List* List_find_node_wb(List* list_ptr, const void* key, int (^elem_comparator)(const void*, const void*));

void List_delete_node(List* list_ptr);
