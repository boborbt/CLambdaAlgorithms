#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _Node {
  Elem elem;
  struct _Node* left;
  struct _Node* right;
} Node;

struct _Dictionary  {
  KeyInfo keyInfo;
  Node* root;
  unsigned int size;
};

typedef struct _Stack {
  Node** array;
  unsigned int size;
  unsigned int capacity;
}* Stack;

struct _DictionaryIterator {
  Stack stack;
};

/* --------------------------
 * Stack implementation
 * -------------------------- */

#define MAX_STACK_SIZE 1024

static Node* Stack_top(Stack stack) {
  if(stack->size==0) {
    return NULL;
  }

  return stack->array[stack->size-1];
}

static Node* Stack_pop(Stack stack) {
  if(stack->size==0) {
    return NULL;
  }

  Node* tmp = stack->array[--stack->size];
  return tmp;
}

static void Stack_push(Stack stack, Node* node) {
  if(stack->size >= stack->capacity) {
    printf("Stack size exceeded");
    exit(1);
  }

  stack->array[stack->size++] = node;
}

static int Stack_empty(Stack stack) {
  return stack->size == 0;
}

static Stack Stack_new(unsigned int capacity) {
  Stack result = (Stack) malloc(sizeof(struct _Stack));
  result->array = (Node**) malloc(sizeof(Node*) * capacity);
  result->size = 0;
  result->capacity = capacity;
  return result;
}

static void Stack_free(Stack stack) {
  free(stack->array);
  free(stack);
}

/* --------------------------
 * DictionaryIterator implementation
 * -------------------------- */

void DictionaryIterator_next(DictionaryIterator it) {
  if(Stack_empty(it->stack)) {
    return;
  }

  Node* cur = Stack_pop(it->stack);
  if(cur->left != NULL) {
    Stack_push(it->stack, cur->left);
  }

  if(cur->right != NULL) {
    Stack_push(it->stack, cur->right);
  }
}

DictionaryIterator DictionaryIterator_new(Dictionary dictionary) {
  DictionaryIterator it = (DictionaryIterator) malloc(sizeof(struct _DictionaryIterator));
  it->stack = Stack_new(MAX_STACK_SIZE);

  Stack_push(it->stack, dictionary->root);
  return it;
}

void DictionaryIterator_free(DictionaryIterator it) {
  Stack_free(it->stack);
  free(it);
}

int DictionaryIterator_end(DictionaryIterator it) {
  return Stack_empty(it->stack);
}

Elem DictionaryIterator_get(DictionaryIterator it) {
  return Stack_top(it->stack)->elem;
}


/* --------------------------
 * Nodes implementation
 * -------------------------- */

static Node* Node_new(void* key, void* value) {
  Node* result =  (Node*) malloc(sizeof(Node));
  result->left = NULL;
  result->right = NULL;
  result->elem = (Elem) malloc(sizeof(struct _Elem));
  result->elem->key = key;
  result->elem->value = value;

  return result;
}

// Find the node storing the given key. Returns a pointer to the variable
// currently pointing to that node. That pointer can be used to modify
// the data structure or to access the contents of the node.
// If no node is found with the given key, returns a pointer to the variable
// that would point to a node with that key.

static Node** Node_find(Node** root, const void* key, KeyInfo keyInfo) {
  Node** node_ptr = root;
  while(*node_ptr != NULL) {
    int comp = KeyInfo_comparator(keyInfo)(key, (*node_ptr)->elem->key);
    if( comp < 0) {
      node_ptr = &(*node_ptr)->left;
      continue;
    }

    if(comp > 0) {
      node_ptr = &(*node_ptr)->right;
      continue;
    }

    break;
  }

  // did not find it, we return the ptr to the variable that should contain
  // a newly inserted node with that key.
  return node_ptr;
}

// static int Node_is_leaf(Node* node) {
//   return node->left == NULL && node->right == NULL;
// }

static Node** Node_find_max(Node** node) {
  if((*node)->right == NULL)
    return node;

  return Node_find_max(&(*node)->right);
}

static void Node_move_key_value(Node* dst, Node* src) {
  if(src == NULL) {
    return;
  }

  dst->elem = src->elem;
  src->elem = NULL;
}

static void Node_delete(Node** node) {
  if((*node)->left == NULL) {
    Node* tmp = *node;
    *node = (*node)->right;
    if(tmp->elem != NULL) {
      free(tmp->elem);
    }
    free(tmp);

    return;
  }

  Node** max_left_ptr = Node_find_max(&(*node)->left);
  Node_move_key_value(*node, *max_left_ptr);
  Node_delete(max_left_ptr);
}

static void Node_tree_free(Node* node) {
  if(node == NULL) {
    return;
  }

  Node_tree_free(node->left);
  Node_tree_free(node->right);
  free(node->elem);
  free(node);
}

// Returns the height of the tree rooted in node. Note that this is a
// O(n) implementation.
static int Node_height(Node* node) {
  if(node == NULL) {
    return 0;
  }

  int lh = Node_height(node->left);
  int rh = Node_height(node->right);

  return 1 + (lh > rh ? lh : rh);
}

/* --------------------------
 * Dictionary implementation
 * -------------------------- */

Dictionary Dictionary_new(KeyInfo keyInfo) {
  Dictionary result = (Dictionary) malloc(sizeof(struct _Dictionary));
  result->keyInfo = keyInfo;
  result->root = NULL;
  result->size = 0;
  return result;
}

void Dictionary_free(Dictionary dictionary) {
  Node_tree_free(dictionary->root);
  free(dictionary);
}

void Dictionary_set(Dictionary dictionary, void* key, void* value) {
  Node** node_ptr = Node_find(&dictionary->root, key, dictionary->keyInfo);

  if((*node_ptr) != NULL) {
    (*node_ptr)->elem->key = key;
    (*node_ptr)->elem->value = value;
  } else {
    *node_ptr = Node_new(key, value);
    dictionary->size += 1;
  }
}

int Dictionary_get(Dictionary dictionary, const void* key, void** value) {
  Node** node_ptr = Node_find(&dictionary->root, key, dictionary->keyInfo);
  if(*node_ptr == NULL) {
    return 0;
  }

  *value = (*node_ptr)->elem->value;
  return 1;
}

void Dictionary_delete(Dictionary dictionary, const void* key) {
  Node** node_ptr = Node_find(&dictionary->root, key, dictionary->keyInfo);
  if(*node_ptr == NULL) {
    return;
  }

  Node_delete(node_ptr);
  dictionary->size -= 1;
}

unsigned int Dictionary_size(Dictionary dictionary) {
  return dictionary->size;
}


double Dictionary_efficiency_score(Dictionary dictionary) {
  return Node_height(dictionary->root);
}
