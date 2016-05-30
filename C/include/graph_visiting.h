#pragma once
#include "graph.h"

typedef struct _VisitingInfo* VisitingInfo;
typedef void* VINode;

// Creates a new visitor for the graph g. The visitors (currently only
// depth_first_visit is implemented) will vist the graph and call the provided
// visit function passint the visited node and the given user info to it).
VisitingInfo VisitingInfo_new(Graph g, void (*visit)(void*, void*), void* user_info);

// Releases the visiting info
void VisitingInfo_free(VisitingInfo info);

// Returns a VINode which has not been visited yet, or NULL if every vertex
// has already been visited
VINode VisitingInfo_next_unvisited(VisitingInfo);

// Returns the VINode corresponding to the given graph vertex. Note: this is
// a costly, O(n), operation.
VINode VisitingInfo_vinode_for_vertex(void* vertex);

// Returns the graph node corresponding to the given VINode
void* VINode_get(VINode);

// Performs a depth first visit
void Graph_depth_first_visit(VisitingInfo info, VINode source);
