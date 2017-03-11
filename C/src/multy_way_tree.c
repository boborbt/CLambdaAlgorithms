#include "multy_way_tree.h"
#include "stack.h"
#include "string_utils.h"
#include <stdio.h>
#include "iterator_functions.h"
#include "mem.h"

struct _MultyWayTree {
  void* content;
  MultyWayTree child;
  MultyWayTree sibling;
};

struct _MultyWayTreeIterator {
  MultyWayTree node;
  Stack state;
};


// constructor
MultyWayTree MultyWayTree_new(void* root_content) {
  MultyWayTree result = (MultyWayTree) Mem_alloc(sizeof(struct _MultyWayTree));
  result->content = root_content;
  result->child = NULL;
  result->sibling = NULL;

  return result;
}

static MultyWayTree* MultyWayTree_anchor_to_first_free_sybling(MultyWayTree* node) {
  MultyWayTree* anchor = node;

  while(*anchor != NULL) {
    anchor = &(*anchor)->sibling;
  }

  return anchor;
}

// Add a child to the given MultyWayTree
void MultyWayTree_add_child(MultyWayTree root, void* content) {
  MultyWayTree child = MultyWayTree_new(content);
  MultyWayTree* anchor = MultyWayTree_anchor_to_first_free_sybling(&root->child);

  *anchor = child;
}

void MultyWayTree_add_subtree(MultyWayTree tree, MultyWayTree subtree) {
  MultyWayTree* anchor = MultyWayTree_anchor_to_first_free_sybling(&tree->child);
  *anchor = subtree;
}

// returns the content of the root node of the given tree
void* MultyWayTree_get(MultyWayTree root) {
  return root->content;
}

// Returns a list of the children of the root of the given tree.
// Each children is itself MultyWayTree and can be updated or queried
// using MultyWayTree operations.
//
// Calling this method on a leaf returns null
// The array (but not its contents) needs to be dealloced by the user
// when it is no longer useful.
Array MultyWayTree_children(MultyWayTree root) {
  MultyWayTree node = root->child;

  if(node == NULL) {
    return NULL;
  }

  Array children = Array_new(10);
  while(node != NULL) {
    Array_add(children, node);
    node = node->sibling;
  }

  return children;
}

MultyWayTree MultyWayTree_find(MultyWayTree tree, void* content, int (*compare)(void*, void*)) {
  if(tree == NULL) {
    return NULL;
  }

  if(compare(MultyWayTree_get(tree),content)==0) {
    return tree;
  }

  MultyWayTree solution = NULL;
  Array children = MultyWayTree_children(tree);
  if(children == NULL) {
    return NULL;
  }

  ArrayIterator it = ArrayIterator_new(children);
  while( !ArrayIterator_end(it) && solution == NULL ) {
    MultyWayTree node = ArrayIterator_get(it);
    solution = MultyWayTree_find(node, content, compare);
    ArrayIterator_next(it);
  }

  ArrayIterator_free(it);
  Array_free(children);

  return solution;
}

// Returns the size (number of nodes) in the tree
size_t MultyWayTree_size(MultyWayTree tree) {
  if(tree == NULL) {
    return 0;
  }

  __block size_t count = 1;
  Array children = MultyWayTree_children(tree);
  if(children == NULL) {
    return 1;
  }

  for_each(Array_it(children), ^(void* elem) {
    MultyWayTree node = elem;

    count += MultyWayTree_size(node);
  });
  Array_free(children);

  return count;
}

// Returns the height (size of the longest path from root to a lief) in the tree
long int MultyWayTree_height(MultyWayTree tree) {
  if(tree == NULL) {
    return -1;
  }

  __block long int result = -1;
  Array children = MultyWayTree_children(tree);
  if(children == NULL) {
    return 0;
  }

  for_each(Array_it(children), ^(void* elem) {
    MultyWayTree child = *(MultyWayTree*) elem;

    long int child_height = MultyWayTree_height(child);
    result =  child_height > result ? child_height : result;
  });

  Array_free(children);

  return result + 1;
}

// Returns the width (size of the level with the highest number of nodes) in the tree
size_t MultyWayTree_max_branching_factor(MultyWayTree tree) {
  if(tree == NULL) {
    return 0;
  }

  Array children = MultyWayTree_children(tree);
  if(children == NULL) {
    return 0;
  }

  __block size_t max_bf = Array_size(children);

  for_each(Array_it(children), ^(void* elem) {
    MultyWayTree child = *(MultyWayTree*) elem;

    size_t child_max_bf = MultyWayTree_max_branching_factor(child);
    max_bf =  max_bf > child_max_bf ? max_bf : child_max_bf;
  });

  Array_free(children);

  return max_bf;
}



void MultyWayTree_free(MultyWayTree root) {
  if(root==NULL) {
    return;
  }

  MultyWayTree_free(root->child);
  root->child = NULL;

  MultyWayTree_free(root->sibling);
  root->sibling = NULL;

  Mem_free(root);
}


// --- Iterator

// Iterator
MultyWayTreeIterator MultyWayTreeIterator_new(MultyWayTree tree) {
  MultyWayTreeIterator iterator = (MultyWayTreeIterator) Mem_alloc(sizeof(struct _MultyWayTreeIterator));
  iterator->state = Stack_new(100);
  Stack_push(iterator->state, tree);
  MultyWayTreeIterator_next(iterator);
  return iterator;
}

void MultyWayTreeIterator_free(MultyWayTreeIterator iterator) {
  Stack_free(iterator->state);
  Mem_free(iterator);
}

// Returns the element currently pointed by the iterator
void* MultyWayTreeIterator_get(MultyWayTreeIterator it) {
  return it->node;
}


// Move the iterator to the next element. Do nothing if it is already past the
// end of the container.
void MultyWayTreeIterator_next(MultyWayTreeIterator it) {
  if(Stack_empty(it->state)) {
    it->node = NULL;
    return;
  }

  it->node = (MultyWayTree) Stack_pop(it->state);
  Array children = MultyWayTree_children(it->node);

  if(children == NULL) {
    return;
  }

  for_each(Array_it(children), ^(void* elem) {
    MultyWayTree child = *(MultyWayTree*) elem;
    Stack_push(it->state, child);
  });

  Array_free(children);
}

// Returns 1 if the iterator is past the end of the container (i.e., if
// MultyWayTreeIterator_get would not return a sensible result), 0 otherwise.
int MultyWayTreeIterator_end(MultyWayTreeIterator it) {
  return it->node == NULL;
}


Iterator MultyWayTree_it(MultyWayTree tree) {

  return Iterator_make(
    tree,
    (void* (*)(void*)) MultyWayTreeIterator_new,
    (void (*)(void*)) MultyWayTreeIterator_next,
    (void* (*)(void*)) MultyWayTreeIterator_get,
    (int (*)(void*)) MultyWayTreeIterator_end,
    (void (*)(void*)) MultyWayTreeIterator_free
  );
}
