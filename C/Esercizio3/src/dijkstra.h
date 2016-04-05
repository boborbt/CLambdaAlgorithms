#pragma once

#include "graph.h"

typedef struct _Dijkstra* Dijkstra;

Dijkstra Dijkstra_new(Graph graph, double (*graph_info_to_double)(const void*));
const void** Dijkstra_minpath(Dijkstra d, const void* source, const void* dest);
void Dijkstra_free(Dijkstra d);
