#include "dictionary.h"
#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "mem.h"

typedef enum {
  BLACK, RED
} Color;

typedef struct _Node {
  KeyValue* kv;
  struct _Node* left;
  struct _Node* right;
  struct _Node* parent;
  Color color;
} Node;

struct _Dictionary  {
  KeyInfo keyInfo;
  Node* root;
  size_t size;
};

struct _DictionaryIterator {
  Stack stack;
};

#define MAX_STACK_SIZE 1024

static Node _nilNode = { .kv = NULL, .left = NULL, .right = NULL, .parent = NULL };
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
  DictionaryIterator it = (DictionaryIterator) Mem_alloc(sizeof(struct _DictionaryIterator));
  it->stack = Stack_new(MAX_STACK_SIZE);

  if(!Dictionary_empty(dictionary)) {
    Stack_push(it->stack, dictionary->root);
  }
  return it;
}

void DictionaryIterator_free(DictionaryIterator it) {
  Stack_free(it->stack);
  Mem_free(it);
}

int DictionaryIterator_end(DictionaryIterator it) {
  return Stack_empty(it->stack);
}

KeyValue* DictionaryIterator_get(DictionaryIterator it) {
  return ((Node*)Stack_top(it->stack))->kv;
}

/* --------------------------
 * Fwd declaration of test utility methods
 * -------------------------- */

int Dictionary_check_parents_structure(Dictionary dictionary);
int Node_check_parents_structure(Node* node);
void Node_dump_tree(Node* node, void (*print_key_value)(void*, void*), char* indent);
void Dictionary_dump(Dictionary dictionary, void (*print_key_value)(void*, void*));
int Dictionary_check_integrity(Dictionary dictionary);


/* --------------------------
 * Nodes implementation
 * -------------------------- */

static Node* Node_new(void* key, void* value) {
  Node* result =  (Node*) Mem_alloc(sizeof(Node));
  result->left = _nil;
  result->right = _nil;
  result->kv = (KeyValue*) Mem_alloc(sizeof(struct _KeyValue*));
  result->kv->key = key;
  result->kv->value = value;
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

static void Node_set_color(Node* node, Color color) {
  if(node == _nil && color == RED) {
    Error_raise(Error_new(ERROR_GENERIC, "Setting color of _nil to red" ));
  }
  node->color = color;
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
    int comp = KeyInfo_comparator(keyInfo)(key, (*node_ptr)->kv->key);
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

  dst->kv = src->kv;
  src->kv = NULL;
}

static Node* Node_delete_non_full_node(Node** node, Node* (*child)(Node*)) {
  Node* tmp = *node;
  *node = child(*node);
  (*node)->parent = tmp->parent;
  Color deleted_color = tmp->color;
  if(tmp->kv != NULL) {
    Mem_free(tmp->kv);
  }
  Mem_free(tmp);

  if(deleted_color == BLACK) {
    return *node;
  } else {
    return NULL;
  }
}

// Removes the given key/value pair from the tree. If the deleted node
// was black then it returns a reference to the node that replaced it.
// If the node was red, it returns NULL.
static Node* Node_delete(Node** node) {
  if((*node)->left == _nil) {
    return Node_delete_non_full_node(node, Node_right);
  }

  Node** max_left_ptr = Node_find_max(&(*node)->left);
  Node_move_key_value(*node, *max_left_ptr);
  return Node_delete_non_full_node(max_left_ptr, Node_left);
}

static void Node_tree_free(Node* node) {
  if(node == _nil) {
    return;
  }

  Node_tree_free(node->left);
  Node_tree_free(node->right);
  Mem_free(node->kv);
  Mem_free(node);
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
  Dictionary result = (Dictionary) Mem_alloc(sizeof(struct _Dictionary));
  result->keyInfo = keyInfo;
  result->root = _nil;
  result->size = 0;

  return result;
}

KeyInfo Dictionary_key_info(Dictionary dictionary) {
  return dictionary->keyInfo;
}


void Dictionary_free(Dictionary dictionary) {
  Node_tree_free(dictionary->root);
  Mem_free(dictionary);
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

static int Dictionary_rb_delete_fixup_cases(Node* w, Node* (*right)(Node*)) {
  if(w->color == RED) {
    return 1;
  }

  if(w==_nil || (w->left->color == BLACK && w->right->color == BLACK)) {
    return 2;
  }

  if(right(w)->color == BLACK) {
    return 3;
  }

  return 4;
}

static Node* Dictionary_rb_delete_fixup_local(
          Dictionary dictionary,
          Node* x,
          Node* (*left)(Node*),
          Node* (*right)(Node*),
          void (*left_rotate)(Dictionary, Node*),
          void (*right_rotate)(Dictionary, Node*)) {
  Node* w;
  // x->parent may change after rotations even when the rotation should not have
  // this effect.
  //
  // This may happen due to the fact that the rotation may involve shared
  // sentinels: when x is a sentinel, and the rotation rotate another node
  // which is itself a sentinel, then x parent may unexpectedly change). We
  // save the pointer here to avoid the problem later on.
  Node* x_parent = x->parent;
  w = right(x_parent);

  switch(Dictionary_rb_delete_fixup_cases(w, right)) {
    case 1:
      Node_set_color(w, BLACK);
      Node_set_color(x->parent, RED);
      w = right(x->parent);
      left_rotate(dictionary, x->parent);
      break;
    case 2:
      Node_set_color(w, RED);
      x = x_parent;
      break;
    case 3:
      Node_set_color(left(w), BLACK);
      Node_set_color(w, RED);
      right_rotate(dictionary, w);
      x->parent = x_parent;
      w = right(x->parent);
      // it falls through 4
    case 4:
      Node_set_color(w, x->parent->color);
      Node_set_color(x->parent, BLACK);
      Node_set_color(right(w), BLACK);
      left_rotate(dictionary, x->parent);
      x = dictionary->root;
      break;
  }
  return x;
}

static void Dictionary_rb_delete_fixup(Dictionary dictionary, Node* x) {
  while( x != dictionary->root && x->color != RED ) {
    if(x==x->parent->left) {
      x = Dictionary_rb_delete_fixup_local(dictionary, x, Node_left, Node_right, Dictionary_rb_left_rotate, Dictionary_rb_right_rotate);
    } else {
      x = Dictionary_rb_delete_fixup_local(dictionary, x, Node_right, Node_left, Dictionary_rb_right_rotate, Dictionary_rb_left_rotate);
    }
  }

  Node_set_color(x, BLACK);
}


static Node* Dictionary_rb_insert_fixup_local(
            Dictionary dictionary, Node* z,
            Node* (*child)(Node*),
            void (*left_rotate)(Dictionary,
            Node*), void (*right_rotate)(Dictionary, Node*)) {

  Node* y = child(z->parent->parent);
  if(y->color == RED) {
    Node_set_color(z->parent, BLACK);
    Node_set_color(y, BLACK);
    Node_set_color(z->parent->parent, RED);
    return z->parent->parent;
  }

  if( z == child(z->parent) ) {
    z = z->parent;
    left_rotate(dictionary, z);
  }

  Node_set_color(z->parent, BLACK);
  Node_set_color(z->parent->parent, RED);
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

  Node_set_color(dictionary->root, BLACK);
}

void Dictionary_set(Dictionary dictionary, void* key, void* value) {
  Node* parent;
  Node** node_ptr = Node_find_with_parent(&dictionary->root, key, dictionary->keyInfo, &parent);

  if((*node_ptr) != _nil) {
    (*node_ptr)->kv->key = key;
    (*node_ptr)->kv->value = value;
  } else {
    *node_ptr = Node_new(key, value);
    (*node_ptr)->parent = parent;
    dictionary->size += 1;
    Dictionary_rb_insert_fixup(dictionary, *node_ptr);
  }
}

int Dictionary_get(Dictionary dictionary, const void* key, void** value) {
  Node** node_ptr = Node_find(&dictionary->root, key, dictionary->keyInfo);
  if(*node_ptr == _nil) {
    return 0;
  }

  if(value!=NULL) {
    *value = (*node_ptr)->kv->value;
  }

  return 1;
}

void Dictionary_delete(Dictionary dictionary, const void* key) {
  Node** node_ptr = Node_find(&dictionary->root, key, dictionary->keyInfo);
  if(*node_ptr == _nil) {
    return;
  }

  Node* node = Node_delete(node_ptr);
  if(node) {
    Dictionary_rb_delete_fixup(dictionary, node);
  }
  dictionary->size -= 1;
}

size_t Dictionary_size(Dictionary dictionary) {
  return dictionary->size;
}


double Dictionary_efficiency_score(Dictionary dictionary) {
  return Node_height(dictionary->root);
}

static int Dictionary_check_black_path_lengths(Node* node);
static int Dictionary_check_red_nodes_children_color(Node* node);
static int Dictionary_check_leaves_are_black(Node* node);
static int Dictionary_check_root_is_black(Dictionary dictionary);


int Dictionary_check_integrity(Dictionary dictionary) {
  if(Dictionary_empty(dictionary)) {
    return 1;
  }

  return
    Dictionary_check_root_is_black(dictionary) &&
    Dictionary_check_leaves_are_black(dictionary->root) &&
    Dictionary_check_red_nodes_children_color(dictionary->root) &&
    Dictionary_check_black_path_lengths(dictionary->root) != -1;
}



// * --------------
// * Test utility methods
// * --------------

void Node_print_address(Node* node);
void Node_dump_colors(Node* node, char* node_name);
void Node_dump_colors_elem(Node* x, char* node_name);


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

  if(node->color == RED && (node->left->color==RED || node->right->color==RED) ) {
    printf("CHK FAILED: node color is red (%p:%d), but left children color:%p:%d right children color: %p:%d\n",
      (void*) node, node->color, (void*)node->left, node->left->color, (void*) node->right, node->right->color);
    return 0;
  }

  return Dictionary_check_red_nodes_children_color(node->left) && Dictionary_check_red_nodes_children_color(node->right);
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

  printf("CHK FAILED. left_black_path_len: %p:%d right_black_path_len: %p:%d\n", (void*) node->left, left_black_path_len, (void*) node->right, right_black_path_len);
  return -1;
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
    printf("_nil%s", _nil->color == BLACK ? "" : "(!RED)");
    return;
  }

  printf("%p", (void*) node);
}

static char* Node_color_string(Node* node) {
  if(node == _nil) {
    return "*BLACK";
  }

  if(node->color == BLACK) {
    return "BLACK";
  } else {
    return "RED";
  }
}

void Node_dump_colors_elem(Node* x, char* node_name) {
  if(x == _nil) {
    printf("%s: _nil\n", node_name);
    return;
  }

  printf("%s %p:%s", node_name, (void*)x, Node_color_string(x));

  if(x->left == _nil) {
    printf(" left: _nil");
  } else {
    printf(" left %p:%s", (void*) x->left, Node_color_string(x->left));
  }

  if(x->right == _nil) {
    printf(" right: _nil\n");
  } else {
    printf(" right %p:%s\n", (void*) x->right, Node_color_string(x->right));
  }
}

void Node_dump_colors(Node* x, char* name) {
  char x_name[1024];
  strcpy(x_name, name);

  Node_dump_colors_elem(x, x_name);

  if(x != _nil) {
    strcpy(x_name, name);
    strcat(x_name, "->left");
    Node_dump_colors_elem(x->left, x_name);

    strcpy(x_name, name);
    strcat(x_name, "->right");
    Node_dump_colors_elem(x->right, x_name);
  }

  if(x->parent == _nil) {
    printf("%s is root\n", name);
    return;
  }

  strcpy(x_name, name);
  strcat(x_name, "->parent");
  Node_dump_colors_elem(x->parent, x_name);

  if(x->parent->left == x) {
    printf("%s == %s->parent->left\n", name, name);
    strcpy(x_name, name);
    strcat(x_name, "->parent->right");
    Node_dump_colors_elem(x->parent->right, x_name);
} else {
    strcpy(x_name, name);
    strcat(x_name, "->parent->left");
    Node_dump_colors_elem(x->parent->left, x_name);
    printf("%s == %s->parent->right\n", name, name);
  }
}

#define INDENT_MAX_SIZE sizeof(char)*1024

void Node_dump_tree(Node* node, void (*print_key_value)(void*, void*), char* indent) {
  char* child_indent = (char*) Mem_alloc(INDENT_MAX_SIZE);
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
  print_key_value(node->kv->key, node->kv->value);
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

  Mem_free(child_indent);
}


void Dictionary_dump(Dictionary dictionary, void (*print_key_value)(void*, void*)) {
  char* indent = (char*) Mem_alloc(INDENT_MAX_SIZE);
  Node_dump_tree(dictionary->root, print_key_value, indent);
}
