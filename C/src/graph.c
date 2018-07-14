#include <stdlib.h>
#include <stdio.h>

#include "graph.h"
#include "dictionary.h"
#include "errors.h"
#include "iterator_functions.h"
#include "mem.h"

// The indices field is a dictionary storing vertices as keys. Values
// are pointers to integers specifying where the key can be found in the
// adj_lists array.
// The adj_lists maintains the adjacency lists for each vertex.
// Each list is implemented with an Array containing EdgeInfo objects.
// This means that a pointer to each source vertex is repeated once
// for each element in its adjacency list. Since this implementation is
// optimizied for sparse graphs, this repetition should not impact much
// the memory consumption of the data structure.
struct _Graph {
  KeyInfo* vertexInfo;
  Dictionary* indices;
  Array* adj_lists;
};

struct _EdgeIterator {
  Graph* graph;
  size_t source_index;
  size_t adj_index;
  int advance_source_index;

  EdgeInfo result;
};

struct _VertexIterator {
 DictionaryIterator* key_iterator;
};


Graph* Graph_new(KeyInfo* vertexInfo) {
  Graph* result = (Graph*) Mem_alloc(sizeof(struct _Graph));
  result->vertexInfo = vertexInfo;
  result->indices = Dictionary_new(vertexInfo);
  result->adj_lists = Array_new(1000);
  return result;
}

void Graph_free(Graph* graph) {
  for_each(Dictionary_it(graph->indices),  ^(void* kv) {
    Mem_free(((KeyValue*)kv)->value);
  });

  for_each(Array_it(graph->adj_lists), ^(void* edge_list) {
    for_each(Array_it(edge_list), ^(void* edge) {
      Mem_free(edge);
    });
    Array_free(edge_list);
  });

  Dictionary_free(graph->indices);
  Array_free(graph->adj_lists);
  Mem_free(graph);
}

KeyInfo* Graph_keyInfo(Graph* graph) {
  return graph->vertexInfo;
}

void Graph_add_vertex(Graph* graph, void* vertex) {
  if(Dictionary_get(graph->indices, vertex, NULL)) {
    Error_raise(Error_new(ERROR_GENERIC, "Trying to insert a vertex twice in the graph"));
  }

  size_t* index = (size_t*) Mem_alloc(sizeof(size_t));
  *index = Array_size(graph->adj_lists);
  Dictionary_set(graph->indices, vertex, index);
  Array_add(graph->adj_lists, Array_new(10));
}


static Array* Graph_adjacents_container(Graph* graph, const void* source) {
  size_t* index;
  if(Dictionary_get(graph->indices, source, (void**)&index) == 0) {
    Error_raise(Error_new(ERROR_GENERIC, "Error: cannot find the given vertex in the graph"));
  }

  return Array_at(graph->adj_lists, *index);
}

void Graph_add_edge(Graph* graph, void* source, void* dest,  void* info) {
  if( !Dictionary_get(graph->indices, dest, NULL) ) {
    Error_raise(Error_new(ERROR_GENERIC, "Error: cannot find the destination vertex in the graph"));
  }

  Array* adj_list = Graph_adjacents_container(graph, source);

  EdgeInfo* edge = (EdgeInfo*) Mem_alloc(sizeof(EdgeInfo));
  edge->source = source;
  edge->destination = dest;
  edge->info = info;

  Array_add(adj_list, edge);
}


size_t Graph_size(Graph* graph) {
  return Array_size(graph->adj_lists);
}

void* Graph_edge_info(Graph* graph, const void* v1, const void* v2) {
  Array* v1_adj_list = Graph_adjacents_container(graph, v1);

  EdgeInfo* edge_info = find_first(Array_it(v1_adj_list), ^(void* obj) {
      EdgeInfo* ei = (EdgeInfo*) obj;
      return KeyInfo_comparator(graph->vertexInfo)(v2, ei->destination)==0;
  });

  if(edge_info==NULL) {
    Error_raise(Error_new(ERROR_GENERIC, "Cannot find v2 in v1 adj list"));
  }

  return edge_info->info;
}

int Graph_has_vertex(Graph* graph, const void* v) {
  return Dictionary_get(graph->indices, v, NULL);
}

int Graph_has_edge(Graph* graph, const void* source, const void* dest) {
  Array* v1_adj_list = Graph_adjacents_container(graph, source);

  return find_first(Array_it(v1_adj_list), ^(void* obj) {
      EdgeInfo* ei = (EdgeInfo*) obj;
      return KeyInfo_comparator(graph->vertexInfo)(ei->destination, dest)==0;
  }) != NULL;
}


// Substitute the edge info in an edge. If the edge is not present in the
// graph it raises an error.
void Graph_set_edge(Graph* graph, void* source, void* dest, void* info) {
  Array* v1_adj_list = Graph_adjacents_container(graph, source);
  EdgeInfo* edge =  find_first(Array_it(v1_adj_list), ^(void* obj) {
      return KeyInfo_comparator(graph->vertexInfo)(obj, dest);
  });

  edge->info = info;
}



EdgeIterator* Graph_adjacents(Graph* graph, void* vertex) {
  size_t* index;
  if(Dictionary_get(graph->indices, vertex, (void**)&index) == 0) {
    Error_raise(Error_new(ERROR_GENERIC, "Error: cannot find the given vertex in the graph"));
  }

  Array* adj_list = Array_at(graph->adj_lists, *index);
  if(Array_size(adj_list) == 0) {
    *index = Array_size(graph->adj_lists);
  }

  EdgeIterator* it = (EdgeIterator*) Mem_alloc(sizeof(struct _EdgeIterator));
  it->graph = graph;
  it->source_index = *index;
  it->adj_index = 0;
  it->advance_source_index = 0;
  return it;
}

VertexIterator* Graph_vertices(Graph* graph) {
  VertexIterator* it = (VertexIterator*) Mem_alloc(sizeof(struct _VertexIterator));
  it->key_iterator = DictionaryIterator_new(graph->indices);
  return it;
}

static size_t Graph_first_vertex_with_adjacents(Graph* graph, size_t vertex_index) {
  size_t graph_size = Array_size(graph->adj_lists);
  if(vertex_index >= graph_size) {
    return vertex_index;
  }

  Array* adj_list = Array_at(graph->adj_lists, vertex_index);
  while(Array_size(adj_list) == 0 && ++vertex_index < graph_size) {
    adj_list = Array_at(graph->adj_lists, vertex_index);
  }

  return vertex_index;
}

EdgeIterator* Graph_edges(Graph* graph) {
  EdgeIterator* it = (EdgeIterator*) Mem_alloc(sizeof(struct _EdgeIterator));
  it->graph = graph;
  it->source_index = Graph_first_vertex_with_adjacents(graph, 0);
  it->adj_index = 0;
  it->advance_source_index = 1;
  return it;
}

void EdgeIterator_free(EdgeIterator* it) {
  Mem_free(it);
}

int EdgeIterator_end(EdgeIterator* it) {
  return it->source_index >= Array_size(it->graph->adj_lists);
}

void EdgeIterator_next(EdgeIterator* it) {
  if(EdgeIterator_end(it)) {
    return;
  }

  it->adj_index += 1;

  void* adj_list = Array_at(it->graph->adj_lists, it->source_index);
  if(it->adj_index < Array_size(adj_list)) {
    return;
  }

  // need to increase the source index
  if(!it->advance_source_index) {
    it->source_index = Array_size(it->graph->adj_lists);
    return;
  }

  it->adj_index = 0;
  it->source_index = Graph_first_vertex_with_adjacents(it->graph, it->source_index + 1);
}



EdgeInfo* EdgeIterator_get(EdgeIterator* it) {
  void* adj_list = Array_at(it->graph->adj_lists, it->source_index);
  return Array_at(adj_list, it->adj_index);
}

int  EdgeIterator_same(EdgeIterator* it1, EdgeIterator* it2) {
  return
    it1->graph           ==  it2->graph &&
    it1->source_index    ==  it2->source_index &&
    it1->adj_index       ==  it2->adj_index;
}


void VertexIterator_free(VertexIterator* it) {
  DictionaryIterator_free(it->key_iterator);
  Mem_free(it);
}

int VertexIterator_end(VertexIterator* it) {
  return DictionaryIterator_end(it->key_iterator);
}

void VertexIterator_next(VertexIterator* it) {
  DictionaryIterator_next(it->key_iterator);
}

void* VertexIterator_get(VertexIterator* it) {
  return (void*) DictionaryIterator_get(it->key_iterator)->key;
}

int VertexIterator_same(VertexIterator* it1, VertexIterator* it2) {
  return DictionaryIterator_same(it1->key_iterator, it2->key_iterator);
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
