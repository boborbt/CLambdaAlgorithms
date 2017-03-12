#pragma once

#include "graph.h"

// Opaque type to support the Dijkstra* algorithm
typedef struct _Dijkstra Dijkstra;

// Creates and initializes the data structures used by the Dijkstra* algorithm
Dijkstra* Dijkstra_new(Graph graph, double (*graph_info_to_double)(const void*));

// Runs Dijkstra* and returns the found minimal path between source and dest
// The minimal path is returned as an array containing a NULL terminated list
// of the nodes found on the minimal path (starting from source and up to dest).
void** Dijkstra_minpath(Dijkstra* d, void* source, void* dest);

// Frees the datastructures used by the Dijkstra* algorithm
void Dijkstra_free(Dijkstra* d);
