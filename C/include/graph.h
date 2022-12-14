#pragma once

#include "keys.h"
#include "iterator.h"

typedef int (*GVertexCompare)(void*, void*);

// Graph* opaque type
typedef struct _Graph Graph;

// EdgeIterator* opaque type
typedef struct _EdgeIterator EdgeIterator;

// VertexIterator* opaque type
typedef struct _VertexIterator VertexIterator;


// Edge information as returned by the EdgeIterator.
typedef struct {
  void* source;
  void* destination;
  void* info;
} EdgeInfo;

// Creates a new graph. Vertices in the graph will be compared and hashed
// using the functions provided by the vertexInfo KeyInfo* structure.
Graph* Graph_new(KeyInfo* vertexInfo);

// Frees the memory alloced by the graph
void Graph_free(Graph* graph);

// Returns the KeyInfo* with which vertices are compared
KeyInfo* Graph_keyInfo(Graph* graph);

// Adds the given vertex to the graph (if it does not already belong to it).
// Raises an ERROR_GENERIC if the vertex already belongs to the graph.
void Graph_add_vertex(Graph* graph, void* vertex);

// Adds the given edge to the graph. Raises a ERROR_GENERIC if the edge
// already belongs to the graph or if either source or dest have not been
// added to the graph (via the Graph_add_vertex method).
void Graph_add_edge(Graph* graph, void* source, void* dest, void* info);

// Returns the number of vertices in the graph
size_t Graph_size(Graph* graph);

// Returns an iterator over all vertices in the graph
VertexIterator* Graph_vertices(Graph* graph);

// Returns an iterator over all edges in the graph. Please not that since
// the edge iterator will provide information about the edge destination only,
// this function is mainly useful if you need to iterate over the list of all
// stored weights. If you need information about source, destination and weight,
// you need to iterate over all vertices (sources) and then get the destinations
// and weights through Graph_adjacencts (alternatively, you can use the
// foreach_graph_edge iterator).
EdgeIterator* Graph_edges(Graph* graph);

// Returns all edges exiting the given vertex
EdgeIterator* Graph_adjacents(Graph* graph, void* vertex);

// Returns the info value associated with the edge connecting v1 and v2.
// If v1 is not in the graph or v2 is not connected to v1, the function
// prints a message and exit(1);
void* Graph_edge_info(Graph* graph, const void* v1, const void* v2);

// Substitute the edge info in an edge. If the edge is not present in the
// graph it raises an error.
void Graph_set_edge(Graph* graph, void* source, void* dest, void* info);

// Returns 1 if the given vertex belongs to the graph. 0 otherwise.
int Graph_has_vertex(Graph* graph, const void* v);

// Returns 1 if the edge source->dest belongs to the graph. 0 otherwise.
int Graph_has_edge(Graph* graph, const void* source, const void* dest);

//
// EDGE ITERATOR
//

// Frees the memory associated with the given edge iterator
void EdgeIterator_free(EdgeIterator* it);

// Returns 1 if the iterator is past the last valid element.
int EdgeIterator_end(EdgeIterator* it);

// Moves the iterator onto the next element.
void EdgeIterator_next(EdgeIterator* it);

// Returns the EdgeInfo currently pointed by the iterator.
EdgeInfo* EdgeIterator_get(EdgeIterator* it);

// Resets the iterator to the first vertex
void EdgeIterator_to_begin(EdgeIterator* it);

// Returns 1 iff it1 and it2 represent the same iterator
int  EdgeIterator_same(EdgeIterator* it1, EdgeIterator* it2);


//
// VERTEX ITERATOR
//


// Frees the memory associated with the given  iterator
void VertexIterator_free(VertexIterator* it);

// Returns 1 if the iterator is past the last valid element.
int VertexIterator_end(VertexIterator* it);

// Moves the iterator onto the next element.
void VertexIterator_next(VertexIterator* it);

// Returns the EdgeInfo currently pointed by the iterator.
void* VertexIterator_get(VertexIterator* it);

// Resets the iterator to the first vertex
void VertexIterator_to_begin(VertexIterator* it);

// Returns 1 iff it1 and it2 represent the same iterator
int  VertexIterator_same(VertexIterator* it1, VertexIterator* it2);

//
// FOREACH methods
//

Iterator Vertex_it(Graph* graph);
Iterator Edge_it(Graph* graph);
Iterator AdjacentsEdge_it(EdgeIterator* it);
