#include "multy_way_tree.h"

struct _MultyWayTree {
  void* content;
  MultyWayTree child;
  MultyWayTree sibling;
};


// constructor
MultyWayTree MultyWayTree_new(void* root_content) {
  MultyWayTree result = (MultyWayTree) malloc(sizeof(struct _MultyWayTree));
  result->content = root_content;
  result->child = NULL;
  result->sibling = NULL;

  return result;
}

// Add a child to the given MultyWayTree
void MultyWayTree_add_child(MultyWayTree root, void* content) {
  MultyWayTree child = MultyWayTree_new(content);
  MultyWayTree* anchor = &root->child;

  while(*anchor != NULL) {
    anchor = &(*anchor)->sibling;
  }

  *anchor = child;
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

  Array children = Array_new(10, sizeof(MultyWayTree));
  while(node != NULL) {
    Array_add(children, &node);
    node = node->sibling;
  }

  return children;
}


void MultyWayTree_free(MultyWayTree root) {
  if(root==NULL) {
    return;
  }

  MultyWayTree_free(root->child);
  root->child = NULL;

  MultyWayTree_free(root->sibling);
  root->sibling = NULL;

  free(root);
}
