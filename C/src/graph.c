#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "dictionary.h"
#include "errors.h"
#include "iterator_functions.h"
#include "mem.h"

// The adjacency matrix is a dictionary storing vertices as keys. Values
// are dictionaries themselves in which keys are the adjacent vertex and
// values are the edge info.
struct _Graph {
  KeyInfo* vertexInfo;
  Dictionary* adjacency_matrix;
  size_t size;
};

struct _EdgeIterator {
  Graph* graph;
  VertexIterator* vertex_it;
  DictionaryIterator* dic_it;
  void* current_source;

  EdgeInfo result;
};

struct _VertexIterator {
 DictionaryIterator* dic_it;
};


Graph* Graph_new(KeyInfo* vertexInfo) {
  Graph* result = (Graph*) Mem_alloc(sizeof(struct _Graph));
  result->vertexInfo = vertexInfo;
  result->adjacency_matrix = Dictionary_new(vertexInfo);
  result->size = 0;
  return result;
}

void Graph_free(Graph* graph) {
  for_each(Dictionary_it(graph->adjacency_matrix),  ^(void* kv) {
    Dictionary_free((Dictionary*) ((KeyValue*)kv)->value);
  });

  Dictionary_free(graph->adjacency_matrix);
  Mem_free(graph);
}

KeyInfo* Graph_keyInfo(Graph* graph) {
  return graph->vertexInfo;
}

void Graph_add_vertex(Graph* graph, void* vertex) {
  if(Dictionary_get(graph->adjacency_matrix, vertex, NULL)) {
    Error_raise(Error_new(ERROR_GENERIC, "Trying to insert a vertex twice in the graph"));
  }

  Dictionary_set(graph->adjacency_matrix, vertex, Dictionary_new(graph->vertexInfo));
  graph->size += 1;
}

static Dictionary* Graph_adjacents_dictionary(Graph* graph, const void* source) {
  Dictionary* adj_list;
  if(Dictionary_get(graph->adjacency_matrix, source, (void**)&adj_list) == 0) {
    Error_raise(Error_new(ERROR_GENERIC, "Error: cannot find the given vertex in the graph"));
  }

  return adj_list;
}

void Graph_add_edge(Graph* graph, void* source, void* dest,  void* info) {
  if( !Dictionary_get(graph->adjacency_matrix, dest, NULL) ) {
    Error_raise(Error_new(ERROR_GENERIC, "Error: cannot find the source vertex in the graph"));
  }

  Dictionary* adj_list = Graph_adjacents_dictionary(graph, source);
  Dictionary_set(adj_list, dest, info);
}

size_t Graph_size(Graph* graph) {
  return graph->size;
}

void* Graph_edge_info(Graph* graph, const void* v1, const void* v2) {
  Dictionary* v1_adj_list = Graph_adjacents_dictionary(graph, v1);
  if(v1_adj_list==NULL) {
    Error_raise(Error_new(ERROR_GENERIC, "Cannot find given vertex"));
  }
  void* info = NULL;
  if(Dictionary_get(v1_adj_list, v2, &info)==0) {
    Error_raise(Error_new(ERROR_GENERIC, "Cannot find v2 in v1 adj list"));
  }
  return info;
}

int Graph_has_vertex(Graph* graph, const void* v) {
  return Dictionary_get(graph->adjacency_matrix, v, NULL);
}

int Graph_has_edge(Graph* graph, const void* source, const void* dest) {
  Dictionary* v1_adj_list = Graph_adjacents_dictionary(graph, source);
  void* info = NULL;
  if(Dictionary_get(v1_adj_list, dest, &info)==0) {
    return 0;
  }
  return 1;
}

// Substitute the edge info in an edge. If the edge is not present in the
// graph it raises an error.
void Graph_set_edge(Graph* graph, void* source, void* dest, void* info) {
  Dictionary* v1_adj_list = Graph_adjacents_dictionary(graph, source);
  Dictionary_set(v1_adj_list, dest, info);
}



EdgeIterator* Graph_adjacents(Graph* graph, void* vertex) {
  Dictionary* adj_list = Graph_adjacents_dictionary(graph, vertex);

  EdgeIterator* it = (EdgeIterator*) Mem_alloc(sizeof(struct _EdgeIterator));
  it->dic_it = DictionaryIterator_new(adj_list);
  it->current_source = vertex;
  it->vertex_it = NULL;
  it->graph = NULL;
  return it;
}

VertexIterator* Graph_vertices(Graph* graph) {
  VertexIterator* it = (VertexIterator*) Mem_alloc(sizeof(struct _VertexIterator));
  it->dic_it = DictionaryIterator_new(graph->adjacency_matrix);
  return it;
}

static void* Graph_first_vertex_with_adjacents(Graph* graph, VertexIterator* vertex_it) {
 DictionaryIterator* dic_it = NULL;
  while(!VertexIterator_end(vertex_it) && dic_it == NULL) {
    Dictionary* adj_list = Graph_adjacents_dictionary(graph, VertexIterator_get(vertex_it));
    if(!Dictionary_empty(adj_list)) {
      return VertexIterator_get(vertex_it);
    } else {
      VertexIterator_next(vertex_it);
    }
  }

  return NULL;
}

EdgeIterator* Graph_edges(Graph* graph) {
  EdgeIterator* it = (EdgeIterator*) Mem_alloc(sizeof(struct _EdgeIterator));
  it->graph = graph;
  it->vertex_it = Graph_vertices(graph);

  void* vertex = Graph_first_vertex_with_adjacents(graph, it->vertex_it);
  it->current_source = vertex;

  if(vertex != NULL) {
    it->dic_it = DictionaryIterator_new( Graph_adjacents_dictionary(graph, vertex) );
  } else {
    it->dic_it = NULL;
  }

  return it;
}

void EdgeIterator_free(EdgeIterator* it) {
  if(it->dic_it!=NULL) {
    DictionaryIterator_free(it->dic_it);
  }

  if(it->vertex_it!=NULL) {
    VertexIterator_free(it->vertex_it);
  }
  Mem_free(it);
}

int EdgeIterator_end(EdgeIterator* it) {
  return it->dic_it == NULL || DictionaryIterator_end(it->dic_it);
}

void EdgeIterator_next(EdgeIterator* it) {
  if(it->dic_it == NULL || DictionaryIterator_end(it->dic_it)) {
    return;
  }

  DictionaryIterator_next(it->dic_it);
  while(it->dic_it != NULL && DictionaryIterator_end(it->dic_it) && it->vertex_it != NULL && !VertexIterator_end(it->vertex_it)) {
    DictionaryIterator_free(it->dic_it);
    VertexIterator_next(it->vertex_it);
    void* vertex = Graph_first_vertex_with_adjacents(it->graph, it->vertex_it);
    it->current_source = vertex;

    if(vertex != NULL) {
      it->dic_it = DictionaryIterator_new(Graph_adjacents_dictionary(it->graph, vertex));
    } else {
      it->dic_it = NULL;
    }
  }
}

EdgeInfo* EdgeIterator_get(EdgeIterator* it) {
  KeyValue* keyValue = DictionaryIterator_get(it->dic_it);

  it->result.source = it->current_source;
  it->result.destination = keyValue->key;
  it->result.info = keyValue->value;

  return &it->result;
}

int  EdgeIterator_same(EdgeIterator* it1, EdgeIterator* it2) {
  return
    it1->graph           ==  it2->graph &&
    VertexIterator_same(it1->vertex_it,it2->vertex_it) &&
    DictionaryIterator_same(it1->dic_it,it2->dic_it) &&
    it1->current_source  ==  it2->current_source;
}


void VertexIterator_free(VertexIterator* it) {
  DictionaryIterator_free(it->dic_it);
  Mem_free(it);
}

int VertexIterator_end(VertexIterator* it) {
  return it->dic_it == NULL || DictionaryIterator_end(it->dic_it);
}

void VertexIterator_next(VertexIterator* it) {
  DictionaryIterator_next(it->dic_it);
}

void* VertexIterator_get(VertexIterator* it) {
  return (void*) DictionaryIterator_get(it->dic_it)->key;
}

int VertexIterator_same(VertexIterator* it1, VertexIterator* it2) {
  return DictionaryIterator_same(it1->dic_it, it2->dic_it);
}


Iterator Vertex_it(Graph* graph) {
 return Iterator_make(
   graph,
   (void*(*)(void*)) Graph_vertices,
   (void (*)(void*))  VertexIterator_next,
   (void*(*)(void*)) VertexIterator_get,
   (int  (*)(void*))   VertexIterator_end,
   (int  (*)(void*, void*)) VertexIterator_same,
   (void (*)(void*))  VertexIterator_free
 );
}


Iterator Edge_it(Graph* graph) {
  return Iterator_make(
    graph,
    (void* (*)(void*)) Graph_edges,
    (void (*)(void*))  EdgeIterator_next,
    (void* (*)(void*)) EdgeIterator_get,
    (int (*)(void*))   EdgeIterator_end,
    (int  (*)(void*, void*)) EdgeIterator_same,
    (void (*)(void*))  EdgeIterator_free
  );
}

static void* AdjacentsEdgeIt_new(EdgeIterator* it) {
  return it;
}

Iterator AdjacentsEdge_it(EdgeIterator* it) {
  return Iterator_make(
  it,
  (void* (*)(void*)) AdjacentsEdgeIt_new,
  (void (*)(void*))  EdgeIterator_next,
  (void* (*)(void*)) EdgeIterator_get,
  (int (*)(void*))   EdgeIterator_end,
  (int  (*)(void*, void*)) EdgeIterator_same,
  (void (*)(void*))  EdgeIterator_free
);
}
