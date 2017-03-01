#pragma once

#include "array.h"

typedef struct _MultyWayTree* MultyWayTree;

// constructor
MultyWayTree MultyWayTree_new(void* root_content);

// Add a child to the given MultyWayTree
void MultyWayTree_add_child(MultyWayTree, void* content);

// returns the content of the root node of the given tree
void* MultyWayTree_get(MultyWayTree);

// Returns a list of the children of the root of the given tree.
// Each children is itself MultyWayTree and can be updated or queried
// using MultyWayTree operations.
//
// Calling this method on a leaf returns null
// The array (but not its contents) needs to be dealloced by the user
// when it is no longer useful.
Array MultyWayTree_children(MultyWayTree);


// Frees the memory alloced by the given tree
void MultyWayTree_free(MultyWayTree);
