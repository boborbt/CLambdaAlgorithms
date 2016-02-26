#ifndef __GRAPH_H_J3HJDL28H__
#define __GRAPH_H_J3HJDL28H__

#include "key_info.h"

typedef int (*GVertexCompare)(void*, void*);

typedef struct _Graph* Graph;
typedef struct _EdgeIterator* EdgeIterator;
typedef struct _VertexIterator* VertexIterator;

typedef struct {
  const void* vertex;
  const void* info;
} EdgeInfo;

Graph Graph_new(KeyInfo vertexInfo);
void Graph_free(Graph graph);

KeyInfo Graph_keyInfo(Graph graph);

// add the given vertex to the graph (if it does not already belong to it)
void Graph_add_vertex(Graph graph, const void* vertex);

// adds the given edge to the graph. Substitute the info value if the edge
// already belongs to the graph.
// if source or dest do not already belong to the graph, it may raise an
// error and exit.
void Graph_add_edge(Graph graph, const void* source, const void* dest, const void* info);

unsigned int Graph_size(Graph graph);

VertexIterator Graph_vertices(Graph graph);
EdgeIterator Graph_adjacents(Graph graph, const void* vertex);

const void* Graph_edge_info(Graph graph, const void* v1, const void* v2);
int Graph_has_vertex(Graph graph, const void* v);
int Graph_has_edge(Graph graph, const void* source, const void* dest);

void EdgeIterator_free(EdgeIterator it);
int EdgeIterator_end(EdgeIterator it);
void EdgeIterator_next(EdgeIterator it);
EdgeInfo EdgeIterator_get(EdgeIterator it);

void VertexIterator_free(VertexIterator it);
int VertexIterator_end(VertexIterator it);
void VertexIterator_next(VertexIterator it);
const void* VertexIterator_get(VertexIterator it);


#endif
