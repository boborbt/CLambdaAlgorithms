#ifndef __DEPTH_FIRST_VISIT_H__
#define __DEPTH_FIRST_VISIT_H__

#include "graph.h"

typedef struct _VisitingInfo* VisitingInfo;

// Creates a new visitor for the graph g. The visitors (currently only
// depth_first_visit is implemented) will vist the graph and call the provided
// visit function passint the visited node and the given user info to it).
VisitingInfo VisitingInfo_new(Graph g, void (*visit)(const void*, void*), void* user_info);

// Releases the visiting info
void VisitingInfo_free(VisitingInfo info);

// Performs a depth first visit
void Graph_depth_first_visit(VisitingInfo info, const void* source);

#endif
