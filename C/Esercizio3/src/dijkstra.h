#ifndef __DIJKSTRA_H_J3HCLWU3N__
#define __DIJKSTRA_H_J3HCLWU3N__

#include "graph.h"

typedef struct _DoubleContainer* DoubleContainer;
typedef struct _Dijkstra* Dijkstra;

DoubleContainer DoubleContainer_new(double value);
void DoubleContainer_free(const DoubleContainer dc);
double DoubleContainer_get(const DoubleContainer dc);


Dijkstra Dijkstra_new(Graph graph, double (*graph_info_to_double)(const void*));
const void** Dijkstra_minpath(Dijkstra d, const void* source, const void* dest);
void Dijkstra_free(Dijkstra d);
#endif
