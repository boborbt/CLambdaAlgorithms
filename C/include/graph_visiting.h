#pragma once
#include "graph.h"

typedef struct _VisitingInfo VisitingInfo;

// Creates a new visitor for the graph g. The visitors will vist the graph and
// call the provided visit function passint the visited node and the given user
// info to it).
VisitingInfo* VisitingInfo_new(Graph* g);

// Releases the visiting info
void VisitingInfo_free(VisitingInfo* info);

// Returns a vertex which has not been visited yet, or NULL if every vertex
// has already been visited. This is useful for visitors (like depth first and
// breadth first visits) that limit their visit to one connected component.
// This method provide an efficient access to the next node to be visited.
// Specifically it has ammortized complexity equal to O(1).
void* VisitingInfo_next_unvisited(VisitingInfo*);

// Performs a depth first visit calling VisitingInfo_callback(info) every time
// a node is explored.
void Graph_depth_first_visit(VisitingInfo* info, void* source, void(^)(void*));

// Performs a breadth first visit calling VisitingInfo_callback(info) every time
// a node is explored.
void Graph_breadth_first_visit(VisitingInfo* info, void* source, void(^)(void*));
