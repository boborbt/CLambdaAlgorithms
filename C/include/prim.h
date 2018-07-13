#pragma once

#include "graph.h"


// Opaque type to support the Dijkstra* algorithm
typedef struct _Prim Prim;

// Creates and initializes the data structures used by the Prim* algorithm
Prim* Prim_new(Graph* graph, double (*graph_info_to_double)(const void*));

// Runs Prim and returns the found minimal spanning forest starting from the given
// node.
// The minimal spanning forest is returned as a newly alloced graph having the same
// KeyInfo* as the source graph
// The user needs to dealloc the graph when he is finished with it.
Graph* Prim_mintree(Prim*, void*);

// Frees the datastructures used by the Prim* algorithm
void Prim_free(Prim*);
