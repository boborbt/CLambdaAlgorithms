#include "dictionary.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>


typedef enum {
  BLACK, RED
} Color;

typedef struct _Node {
  Elem elem;
  struct _Node* left;
  struct _Node* right;
  struct _Node* parent;
  Color color;
} Node;

struct _Dictionary  {
  KeyInfo keyInfo;
  Node* root;
  unsigned int size;
};

struct _DictionaryIterator {
  Stack stack;
};

#define MAX_STACK_SIZE 1024

static Node _nilNode = { .elem = NULL, .left = NULL, .right = NULL, .parent = NULL };
static Node* _nil = &_nilNode;

/* --------------------------
 * DictionaryIterator implementation
 * -------------------------- */

void DictionaryIterator_next(DictionaryIterator it) {
  if(Stack_empty(it->stack)) {
    return;
  }

  Node* cur = Stack_pop(it->stack);
  if(cur->left != _nil) {
    Stack_push(it->stack, cur->left);
  }

  if(cur->right != _nil) {
    Stack_push(it->stack, cur->right);
  }
}

DictionaryIterator DictionaryIterator_new(Dictionary dictionary) {
  DictionaryIterator it = (DictionaryIterator) malloc(sizeof(struct _DictionaryIterator));
  it->stack = Stack_new(MAX_STACK_SIZE);

  if(!Dictionary_empty(dictionary)) {
    Stack_push(it->stack, dictionary->root);
  }
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
  return ((Node*)Stack_top(it->stack))->elem;
}

/* --------------------------
 * Fwd declaration of test utility methods
 * -------------------------- */

int Dictionary_check_parents_structure(Dictionary dictionary);
int Node_check_parents_structure(Node* node);
void Node_dump_tree(Node* node, void (*print_key_value)(void*, void*), char* indent);
void Dictionary_dump(Dictionary dictionary, void (*print_key_value)(void*, void*));
int Dictionary_check_rb_properties(Dictionary dictionary);


/* --------------------------
 * Nodes implementation
 * -------------------------- */

static Node* Node_new(void* key, void* value) {
  Node* result =  (Node*) malloc(sizeof(Node));
  result->left = _nil;
  result->right = _nil;
  result->elem = (Elem) malloc(sizeof(struct _Elem));
  result->elem->key = key;
  result->elem->value = value;
  result->color = RED;
  result->parent = _nil;

  return result;
}

static Node* Node_left(Node* node) {
  return node->left;
}

static Node* Node_right(Node* node) {
  return node->right;
}

// Sets properties of the given node. In particular, the parent, left and right
// pointers are set as specified. Note that the left and right parents are also
// set to point to the given node, while the parent node pointer is *not*
// updated to point to node (it cannot be done in this method because the
// parameters provide no information  about wheter node should be the left or
// the right child of parent).
static void Node_set(Node* node, Node* parent, Node* left, Node* right) {
  node->parent = parent;

  node->left = left;
  node->left->parent = node;

  node->right = right;
  node->right->parent = node;
}

// Given a node accesses its parent to find which of his children is x. Then
// returns a pointer to that branch. That pointer can be then used to reassign
// that branch to another node.
static Node** Node_parent_ptr_to(Node* x) {
  if(x->parent->left == x) {
    return &x->parent->left;
  } else {
    return &x->parent->right;
  }
}

// Just like Node_find, but fills parent with a pointer to the parent of the found node.
static Node** Node_find_with_parent(Node** root, const void* key, KeyInfo keyInfo, Node** parent) {
  (*parent) = _nil;
  Node** node_ptr = root;

  while(*node_ptr != _nil) {
    int comp = KeyInfo_comparator(keyInfo)(key, (*node_ptr)->elem->key);
    if( comp < 0) {
      *parent = *node_ptr;
      node_ptr = &(*node_ptr)->left;
      continue;
    }

    if(comp > 0) {
      *parent = *node_ptr;
      node_ptr = &(*node_ptr)->right;
      continue;
    }

    break;
  }

  // did not find it, we return the ptr to the variable that should contain
  // a newly inserted node with that key.
  return node_ptr;
}

// Find the node storing the given key. Returns a pointer to the variable
// currently pointing to that node. That pointer can be used to modify
// the data structure or to access the contents of the node.
// If no node is found with the given key, returns a pointer to the variable
// that would point to a node with that key.

static Node** Node_find(Node** root, const void* key, KeyInfo keyInfo) {
  Node* parent;
  return Node_find_with_parent(root, key, keyInfo, &parent);
}

// RB tree leaves are _nil sentinels.
static int Node_is_leaf(Node* node) {
  return node == _nil;
}

static Node** Node_find_max(Node** node) {
  if((*node)->right == _nil)
    return node;

  return Node_find_max(&(*node)->right);
}

static void Node_move_key_value(Node* dst, Node* src) {
  if(src == _nil) {
    return;
  }

  dst->elem = src->elem;
  src->elem = NULL;
}

static void Node_delete(Node** node) {
  if((*node)->left == _nil) {
    Node* tmp = *node;
    *node = (*node)->right;
    (*node)->parent = tmp->parent;

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
  if(node == _nil) {
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
  if(node == _nil) {
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
  result->root = _nil;
  result->size = 0;

  return result;
}

void Dictionary_free(Dictionary dictionary) {
  Node_tree_free(dictionary->root);
  free(dictionary);
}

static void Dictionary_rb_left_rotate(Dictionary dictionary, Node* node) {
  Node* x = node;
  Node* y = x->right;
  Node* a = x->left;
  Node* b = y->left;
  Node* c = y->right;

  *Node_parent_ptr_to(x) = y;

  Node_set(y, x->parent, x, c);
  Node_set(x, y, a, b);

  if(y->parent == _nil) {
    dictionary->root = y;
  }
}

static void Dictionary_rb_right_rotate(Dictionary dictionary, Node* node) {
  Node* y = node;
  Node* x = y->left;
  Node* a = x->left;
  Node* b = x->right;
  Node* c = y->right;

  *Node_parent_ptr_to(y) = x;

  Node_set(x, y->parent, a, y);
  Node_set(y, x, b, c);

  if(x->parent == _nil) {
    dictionary->root = x;
  }
}


static Node* Dictionary_rb_insert_fixup_local(
            Dictionary dictionary, Node* z,
            Node* (*child)(Node*),
            void (*left_rotate)(Dictionary,
            Node*), void (*right_rotate)(Dictionary, Node*)) {

  Node* y = child(z->parent->parent);
  if(y->color == RED) {
    z->parent->color = BLACK;
    y->color = BLACK;
    z->parent->parent->color = RED;
    return z->parent->parent;
  }

  if( z == child(z->parent) ) {
    z = z->parent;
    left_rotate(dictionary, z);
  }

  z->parent->color = BLACK;
  z->parent->parent->color = RED;
  right_rotate(dictionary, z->parent->parent);

  return z;
}

static void Dictionary_rb_insert_fixup(Dictionary dictionary, Node* z) {
  while(z->parent->color == RED) {
    if(z->parent == z->parent->parent->left) {
      z = Dictionary_rb_insert_fixup_local(dictionary, z, Node_right, Dictionary_rb_left_rotate, Dictionary_rb_right_rotate);
    } else {
      z = Dictionary_rb_insert_fixup_local(dictionary, z, Node_left, Dictionary_rb_right_rotate, Dictionary_rb_left_rotate);
    }
  }

  dictionary->root->color = BLACK;
}

void Dictionary_set(Dictionary dictionary, void* key, void* value) {
  Node* parent;
  Node** node_ptr = Node_find_with_parent(&dictionary->root, key, dictionary->keyInfo, &parent);

  if((*node_ptr) != _nil) {
    (*node_ptr)->elem->key = key;
    (*node_ptr)->elem->value = value;
  } else {
    *node_ptr = Node_new(key, value);
    (*node_ptr)->parent = parent;
    dictionary->size += 1;
  }

  Dictionary_rb_insert_fixup(dictionary, *node_ptr);
}

int Dictionary_get(Dictionary dictionary, const void* key, void** value) {
  Node** node_ptr = Node_find(&dictionary->root, key, dictionary->keyInfo);
  if(*node_ptr == _nil) {
    return 0;
  }

  *value = (*node_ptr)->elem->value;
  return 1;
}

void Dictionary_delete(Dictionary dictionary, const void* key) {
  Node** node_ptr = Node_find(&dictionary->root, key, dictionary->keyInfo);
  if(*node_ptr == _nil) {
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



// * --------------
// * Test utility methods
// * --------------

static int Dictionary_check_root_is_black(Dictionary dictionary) {
  if(dictionary->root->color != BLACK) {
    printf("CHK FAILED: root color is %d\n", dictionary->root->color);
    return 0;
  }

  return 1;
}

static int Dictionary_check_leaves_are_black(Node* node) {
  if(node == _nil) {
    return 1;
  }

  if(Node_is_leaf(node)) {
    if(node->color != BLACK) {
      printf("CHK FAILED: node is a leaf, but has color: %d\n", node->color);
      return 0;
    }

    return 1;
  }

  return Dictionary_check_leaves_are_black(node->left) && Dictionary_check_leaves_are_black(node->right);
}

static int Dictionary_check_red_nodes_children_color(Node* node) {
  if(node == _nil) {
    return 1;
  }

  if(node->color == RED && (node->left->color==BLACK || node->right->color==BLACK) ) {
    printf("CHK FAILED: node color is red (%d), but left children color:%d right children color: %d\n",
      node->color, node->left->color, node->right->color);
    return 0;
  }

  return Dictionary_check_leaves_are_black(node->left) && Dictionary_check_leaves_are_black(node->right);
}

static int Dictionary_check_black_path_lengths(Node* node) {
  if(node == _nil) {
    return 1;
  }

  int left_black_path_len = Dictionary_check_black_path_lengths(node->left);
  int right_black_path_len = Dictionary_check_black_path_lengths(node->right);

  if( left_black_path_len == right_black_path_len && left_black_path_len != -1 ) {
    return left_black_path_len + (node->color==BLACK ? 1 : 0);
  }

  printf("CHK FAILED. left_black_path_len: %d right_black_path_len: %d\n", left_black_path_len, right_black_path_len);
  return -1;
}

int Dictionary_check_rb_properties(Dictionary dictionary) {
  if(Dictionary_empty(dictionary)) {
    return 1;
  }

  return
    Dictionary_check_root_is_black(dictionary) &&
    Dictionary_check_leaves_are_black(dictionary->root) &&
    Dictionary_check_red_nodes_children_color(dictionary->root) &&
    Dictionary_check_black_path_lengths(dictionary->root) != -1;
}


int Dictionary_check_parents_structure(Dictionary dictionary) {
  if(dictionary->root == _nil) {
    return 1;
  }

  if(dictionary->root->parent != _nil) {
    return 0;
  }

  return Node_check_parents_structure(dictionary->root);
}


void Node_print_address(Node* node);

int Node_check_parents_structure(Node* node) {
  if( node == _nil ) {
    return 1;
  }
  if( Node_is_leaf(node) ) {
    return 1;
  }

  int left_check =
    node->left == _nil || (
      node->left->parent == node &&
      Node_check_parents_structure(node->left)
    );

  int right_check =
    node->right == _nil || (
      node->right->parent == node &&
      Node_check_parents_structure(node->right)
    );

  if( !(left_check && right_check) ) {
    printf("check fails on node:");
    Node_print_address(node);
    printf("\n");
  }

  return left_check && right_check;
}

void Node_print_address(Node* node) {
  if(node == NULL) {
    printf("NULL");
    return;
  }

  if(node == _nil) {
    printf("_nil");
    return;
  }

  printf("%p", (void*) node);
}

#define INDENT_MAX_SIZE sizeof(char)*1024

void Node_dump_tree(Node* node, void (*print_key_value)(void*, void*), char* indent) {
  char* child_indent = (char*) malloc(INDENT_MAX_SIZE);
  strcpy(child_indent, indent);
  strcat(child_indent, "  ");

  if(node == _nil) {
    Node_print_address(node);
    return;
  }

  printf("\n%sself:", indent);
  Node_print_address(node);
  printf("\n");

  printf("%skey/value:", indent);
  print_key_value(node->elem->key, node->elem->value);
  printf("\n");

  printf("%scolor:%d\n", indent, node->color);

  printf("%sparent:", indent);
  Node_print_address(node->parent);
  printf("\n");

  printf("%sleft:", indent);
  Node_dump_tree(node->left, print_key_value, child_indent);
  printf("\n");

  printf("%sright:", indent);
  Node_dump_tree(node->right, print_key_value, child_indent);
  printf("\n");

  free(child_indent);
}

void Dictionary_dump(Dictionary dictionary, void (*print_key_value)(void*, void*)) {
  char* indent = (char*) malloc(INDENT_MAX_SIZE);
  Node_dump_tree(dictionary->root, print_key_value, indent);
}
