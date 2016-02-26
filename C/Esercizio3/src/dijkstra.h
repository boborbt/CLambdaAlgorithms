#ifndef __DIJKSTRA_H_J3HCLWU3N__
#define __DIJKSTRA_H_J3HCLWU3N__

#include "graph.h"

typedef struct _DoubleContainer* DoubleContainer;

DoubleContainer DoubleContainer_new(double value);
void DoubleContainer_free(const DoubleContainer dc);
double DoubleContainer_get(const DoubleContainer dc);

const void** dijkstra(Graph graph,
                const void* source,
                const void* dest, double (*graph_info_to_double)(const void*));

#endif
