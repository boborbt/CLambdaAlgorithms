#pragma once

#include "graph.h"


// Opaque type to support the Dijkstra* algorithm
typedef struct _Kruskal* Kruskal;

// Creates and initializes the data structures used by the Dijkstra* algorithm
Kruskal Kruskal_new(Graph* graph, double (*graph_info_to_double)(const void*));

// Runs Kruskal and returns the found minimal spanning tree
// The minimal path is returned as a newly alloced graph having the same
// KeyInfo* as the source graph
// The user needs to dealloc the graph when finished with it.
Graph* Kruskal_mintree(Kruskal);

// Frees the datastructures used by the Kruskal algorithm
void Kruskal_free(Kruskal);
