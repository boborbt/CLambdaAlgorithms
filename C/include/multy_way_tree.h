#pragma once

#include "array.h"

typedef struct _MultyWayTree* MultyWayTree;
typedef struct _MultyWayTreeIterator* MultyWayTreeIterator;

// constructor
MultyWayTree MultyWayTree_new(void* root_content);
// Frees the memory alloced by the given tree
void MultyWayTree_free(MultyWayTree);


// Add a child to the given MultyWayTree
void MultyWayTree_add_child(MultyWayTree, void* content);

// Add the given subtree as a child of the given tree
void MultyWayTree_add_subtree(MultyWayTree tree, MultyWayTree subtree);

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

// Returns the first node (i.e. the root of the first subtree) encountered that satisfies that its
// content equals the given content as predicated by the compare function. Returns NULL if such node
// does not exists.
//
// The compare function needs to respect the "strcmp" protocol, i.e., it needs to return
// 0 if the elements are equal, -1 if the first is smaller than the second and 1 otherwise.
MultyWayTree MultyWayTree_find(MultyWayTree tree, void* content, int (*compare)(void*, void*));

// Returns the size (number of nodes) in the tree
size_t MultyWayTree_size(MultyWayTree);

// Returns the height (size of the longest path from root to a lief) in the tree
// A tree with only the root has, by definition, height 0;
// A NULL tree has, by definition, height -1
long int MultyWayTree_height(MultyWayTree);

// Returns the width (size of the level with the highest number of nodes) in the tree
size_t MultyWayTree_max_branching_factor(MultyWayTree);

// -- ITERATOR --
// Iterator
MultyWayTreeIterator MultyWayTreeIterator_new(MultyWayTree);
void MultyWayTreeIterator_free(MultyWayTreeIterator);

// Move the iterator to the next element. Do nothing if it is already past the
// end of the container.
void MultyWayTreeIterator_next(MultyWayTreeIterator it);

// Returns 1 if the iterator is past the end of the container (i.e., if
// MultyWayTreeIterator_get would not return a sensible result), 0 otherwise.
int MultyWayTreeIterator_end(MultyWayTreeIterator it);

// Returns the element currently pointed by the iterator
void* MultyWayTreeIterator_get(MultyWayTreeIterator it);

//
// FOREACH
//
void foreach_multy_way_tree_elem(MultyWayTree multy_way_tree, void (^)(void*));
