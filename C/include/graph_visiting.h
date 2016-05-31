#pragma once
#include "graph.h"

typedef struct _VisitingInfo* VisitingInfo;

// Creates a new visitor for the graph g. The visitors (currently only
// depth_first_visit is implemented) will vist the graph and call the provided
// visit function passint the visited node and the given user info to it).
VisitingInfo VisitingInfo_new(Graph g);

// Releases the visiting info
void VisitingInfo_free(VisitingInfo info);

// Returns a vertex which has not been visited yet, or NULL if every vertex
// has already been visited
void* VisitingInfo_next_unvisited(VisitingInfo);

// Performs a depth first visit calling VisitingInfo_callback(info) every time
// a node is explored.
void Graph_depth_first_visit(VisitingInfo info, void* source, void(^)(void*));
