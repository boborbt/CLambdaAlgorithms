#include "dictionary.h"
#include <stdlib.h>

typedef struct _Node {
  void* key;
  void* value;
  struct _Node* left;
  struct _Node* right;
} Node;

struct _Dictionary  {
  KeyComparator comparator;
  Node* root;
  unsigned int size;
};


Node* Node_new(void* key, void* value) {
  Node* result =  (Node*) malloc(sizeof(Node));
  result->left = NULL;
  result->right = NULL;
  result->key = key;
  result->value = value;

  return result;
}

Node** Node_find(Node** root, void* key, KeyComparator comparator) {
  Node** node_ptr = root;
  while(*node_ptr != NULL) {
    int comp = comparator(key, (*node_ptr)->key);
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

int Node_is_leaf(Node* node) {
  return node->left == NULL && node->right == NULL;
}

Node** Node_find_max(Node** node) {
  if((*node)->right == NULL)
    return node;

  return Node_find_max(&(*node)->right);
}

void Node_copy_key_value(Node* dst, Node* src) {
  if(src == NULL) {
    return;
  }

  dst->key = src->key;
  dst->value = src->value;
}

void Node_delete(Node** node) {
  if(*node == NULL) {
    return;
  }

  if((*node)->left == NULL) {
    Node* tmp = *node;
    *node = (*node)->right;
    free(tmp);

    return;
  }

  Node** max_left_ptr = Node_find_max(&(*node)->left);
  Node_copy_key_value(*node, *max_left_ptr);
  Node_delete(max_left_ptr);
}

void Node_tree_free(Node* node) {
  if(node == NULL) {
    return;
  }

  Node_tree_free(node->left);
  Node_tree_free(node->right);
  free(node);
}

int Node_height(Node* node) {
  if(node == NULL) {
    return 0;
  }

  int lh = Node_height(node->left);
  int rh = Node_height(node->right);

  return 1 + (lh > rh ? lh : rh);
}

Dictionary Dictionary_new(KeyComparator comparator) {
  Dictionary result = (Dictionary) malloc(sizeof(struct _Dictionary));
  result->comparator = comparator;
  result->root = NULL;
  result->size = 0;
  return result;
}

void Dictionary_free(Dictionary dictionary) {
  Node_tree_free(dictionary->root);
  free(dictionary);
}

void Dictionary_set(Dictionary dictionary, void* key, void* value) {
  Node** node_ptr = Node_find(&dictionary->root, key, dictionary->comparator);

  if((*node_ptr) != NULL) {
    (*node_ptr)->value = value;
  } else {
    *node_ptr = Node_new(key, value);
    dictionary->size += 1;
  }
}

int Dictionary_get(Dictionary dictionary, void* key, void** value) {
  Node** node_ptr = Node_find(&dictionary->root, key, dictionary->comparator);
  if(*node_ptr == NULL) {
    return 0;
  }

  *value = (*node_ptr)->value;
  return 1;
}

void Dictionary_delete(Dictionary dictionary, void* key) {
  Node** node_ptr = Node_find(&dictionary->root, key, dictionary->comparator);
  if(*node_ptr == NULL) {
    return;
  }

  Node_delete(node_ptr);
  dictionary->size -= 1;
}

unsigned int Dictionary_size(Dictionary dictionary) {
  return dictionary->size;
}


int Dictionary_efficiency_score(Dictionary dictionary) {
  return Node_height(dictionary->root);
}
