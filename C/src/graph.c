#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "dictionary.h"

// The adjacency matrix is a dictionary storing vertices as keys. Values
// are dictionaries themselves in which keys are the adjacent vertex and
// values are the edge info.
struct _Graph {
  KeyInfo vertexInfo;
  Dictionary adjacency_matrix;
  unsigned int size;
};

struct _EdgeIterator {
  DictionaryIterator dic_it;
};

struct _VertexIterator {
  DictionaryIterator dic_it;
};


Graph Graph_new(KeyInfo vertexInfo) {
  Graph result = (Graph) malloc(sizeof(struct _Graph));
  result->vertexInfo = vertexInfo;
  result->adjacency_matrix = Dictionary_new(vertexInfo);
  result->size = 0;
  return result;
}

void Graph_free(Graph graph) {
  Dictionary_free(graph->adjacency_matrix);
  free(graph);
}

KeyInfo Graph_keyInfo(Graph graph) {
  return graph->vertexInfo;
}

void Graph_add_vertex(Graph graph, const void* vertex) {
  const void* dummy;
  if(Dictionary_get(graph->adjacency_matrix, vertex, &dummy)) {
    printf("Error: Trying to insert a vertex twice in the graph");
    exit(1);
  }
  Dictionary_set(graph->adjacency_matrix, vertex, Dictionary_new(graph->vertexInfo));
  graph->size += 1;
}

Dictionary Graph_adjacents_dictionary(Graph graph, const void* source) {
  Dictionary adj_list;
  if(Dictionary_get(graph->adjacency_matrix, source, (const void**)&adj_list) == 0) {
    printf("Error: cannot find the given vertex in the graph");
    exit(1);
  }

  return adj_list;
}

void Graph_add_edge(Graph graph, const void* source, const void* dest, const void* info) {
  Dictionary adj_list = Graph_adjacents_dictionary(graph, source);
  Dictionary_set(adj_list, dest, info);
}

unsigned int Graph_size(Graph graph) {
  return graph->size;
}

const void* Graph_edge_info(Graph graph, const void* v1, const void* v2) {
  Dictionary v1_adj_list = Graph_adjacents_dictionary(graph, v1);
  if(v1_adj_list==NULL) {
    printf("Cannot find given vertex\n");
    exit(1);
  }
  const void* info = NULL;
  if(Dictionary_get(v1_adj_list, v2, &info)==0) {
    printf("Cannot find v2 in v1 adj list\n");
    exit(1);
  }
  return info;
}

EdgeIterator Graph_adjacents(Graph graph, const void* vertex) {
  Dictionary adj_list = Graph_adjacents_dictionary(graph, vertex);

  EdgeIterator it = (EdgeIterator) malloc(sizeof(struct _EdgeIterator));
  it->dic_it = DictionaryIterator_new(adj_list);
  return it;
}

VertexIterator Graph_vertices(Graph graph) {
  VertexIterator it = (VertexIterator) malloc(sizeof(struct _VertexIterator));
  it->dic_it = DictionaryIterator_new(graph->adjacency_matrix);
  return it;
}

void EdgeIterator_free(EdgeIterator it) {
  DictionaryIterator_free(it->dic_it);
  free(it);
}

int EdgeIterator_end(EdgeIterator it) {
  return DictionaryIterator_end(it->dic_it);
}

void EdgeIterator_next(EdgeIterator it) {
  DictionaryIterator_next(it->dic_it);
}

EdgeInfo EdgeIterator_get(EdgeIterator it) {
  Elem keyValue = DictionaryIterator_get(it->dic_it);

  EdgeInfo result;
  result.vertex = keyValue->key;
  result.info = keyValue->value;

  return result;
}

void VertexIterator_free(VertexIterator it) {
  DictionaryIterator_free(it->dic_it);
  free(it);
}

int VertexIterator_end(VertexIterator it) {
  return DictionaryIterator_end(it->dic_it);
}

void VertexIterator_next(VertexIterator it) {
  DictionaryIterator_next(it->dic_it);
}

const void* VertexIterator_get(VertexIterator it) {
  return DictionaryIterator_get(it->dic_it)->key;
}
