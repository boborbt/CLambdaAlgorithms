#ifndef __GRAPH_H_J3HJDL28H__
#define __GRAPH_H_J3HJDL28H__

#include "key_info.h"

typedef int (*GVertexCompare)(void*, void*);

typedef struct _Graph* Graph;
typedef struct _EdgeIterator* EdgeIterator;
typedef struct _VertexIterator* VertexIterator;

typedef struct {
  void* vertex;
  void* info;
} EdgeInfo;

Graph Graph_new(KeyInfo vertexInfo);

KeyInfo Graph_keyInfo(Graph graph);

// add the given vertex to the graph (if it does not already belong to it)
void Graph_add_vertex(Graph graph, void* vertex);

// adds the given edge to the graph. Substitute the info value if the edge
// already belongs to the graph.
// if source or dest do not already belong to the graph, it may raise an
// error and exit.
void Graph_add_edge(Graph graph, void* source, void* dest, void* info);

unsigned int Graph_size(Graph graph);

VertexIterator Graph_vertices(Graph graph);
EdgeIterator Graph_adjacents(Graph graph, void* vertex);

void EdgeIterator_free(EdgeIterator it);
int EdgeIterator_end(EdgeIterator it);
void EdgeIterator_next(EdgeIterator it);
EdgeInfo EdgeIterator_get(EdgeIterator it);

void VertexIterator_free(VertexIterator it);
int VertexIterator_end(VertexIterator it);
void VertexIterator_next(VertexIterator it);
void* VertexIterator_get(VertexIterator it);


#endif
