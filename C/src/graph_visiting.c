#include "graph_visiting.h"
#include "dictionary.h"
#include "union_find.h"
#include <stdlib.h>
#include "errors.h"
#include "queue.h"
#include "mem.h"
#include "iterator_functions.h"

struct _VisitingInfo {
  Graph* graph;

  Dictionary* vertex_set;
  UnionFindSet* visited_set;
 DictionaryIterator* current;
};


VisitingInfo* VisitingInfo_new(Graph* graph) {
  VisitingInfo* result = (VisitingInfo*) Mem_alloc(sizeof(struct _VisitingInfo));

  result->graph = graph;
  result->vertex_set = Dictionary_new(Graph_keyInfo(graph));
  for_each(Vertex_it(result->graph), ^(void* vertex){
    Dictionary_set(result->vertex_set, vertex, UnionFindSet_new(vertex));
  });
  result->current = DictionaryIterator_new(result->vertex_set);
  result->visited_set = UnionFindSet_new(NULL);

  return result;
}

void VisitingInfo_free(VisitingInfo* info) {
  UnionFindSet_free(info->visited_set);
  DictionaryIterator_free(info->current);
  for_each(Dictionary_it(info->vertex_set),  ^(void* kv) {
    UnionFindSet_free(((KeyValue*)kv)->value);
  });
  Dictionary_free(info->vertex_set);

  Mem_free(info);
}

// Returns the next unvisited node. Thanks to the UnionFindSet* we have O(1)
// check if a node has been visited. Also visiting info maintain an iterator
// over vertices which allows to traverse the list of all vertices exactly once.
// Counting all possible calls to this function over to explore a graph, the
// cumulative time is therefore O(n) in the worst and in the best case. This is
// assuming O(1) Dictionary_get. A naive implementation not using the union set
// and traversing each time the dictionary in the search of the next unvisited
// node would require O(n) time in the best case, and O(n^2) in the worst.
void* VisitingInfo_next_unvisited(VisitingInfo* info) {
  if(DictionaryIterator_end(info->current)) {
    return NULL;
  }

  UnionFindSet* result = DictionaryIterator_get(info->current)->value;
  DictionaryIterator_next(info->current);

  while( UnionFindSet_same(result, info->visited_set) && !DictionaryIterator_end(info->current)) {
    result = DictionaryIterator_get(info->current)->value;
    DictionaryIterator_next(info->current);
  }

  if( UnionFindSet_same(result, info->visited_set) ) {
    return NULL;
  }

  return UnionFindSet_get(result);
}

// This is the real depth first visit. The visit is performed on UnionFindSet
// wrapped nodes, assumes that the source is this kind of node, and calls itself
// recursively on this kind of nodes. The outer visit only serves as a wrapper
// which recover the UnionFindSet* node corresponding to the given vertex.
static void Graph_depth_first_visit_uf_set(VisitingInfo* info, UnionFindSet* source, void (^visit)(void*)) {
  UnionFindSet_union(source, info->visited_set);
  visit(UnionFindSet_get(source));

  EdgeIterator* adjacents = Graph_adjacents(info->graph, UnionFindSet_get(source));
  for_each(AdjacentsEdge_it(adjacents), ^(void* obj) {
    EdgeInfo* ei = (EdgeInfo*) obj;
    void* neighbour = ei->destination;
    UnionFindSet* neighbour_set;

    if(!Dictionary_get(info->vertex_set, neighbour, (void**)&neighbour_set)) {
      Error_raise( Error_new( ERROR_GENERIC, "Cannot find searched vertex" ));
    }

    if(!UnionFindSet_same(neighbour_set, info->visited_set)) {
      Graph_depth_first_visit_uf_set(info, neighbour_set, visit);
    }
  });
}

// Finds the UnionFindSet* corresponding a source_vertex and calls the
// DFS based on UnionFindSet* nodes.
void Graph_depth_first_visit(VisitingInfo* info, void* source_vertex, void (^visit)(void*)) {
  UnionFindSet* source;

  if(!Dictionary_get(info->vertex_set, source_vertex, (void**)&source)) {
    Error_raise( Error_new(ERROR_GENERIC, "DFS: cannot find given vertex") );
  }

  Graph_depth_first_visit_uf_set(info, source, visit);
}

void Graph_breadth_first_visit(VisitingInfo* info, void* source_vertex, void (^visit)(void*)) {
  UnionFindSet* source;

  if(!Dictionary_get(info->vertex_set, source_vertex, (void**)&source)) {
    Error_raise( Error_new(ERROR_GENERIC, "BFS: cannot find given vertex") );
  }

  Queue* queue = Queue_new();
  Queue_enqueue(queue, source);

  while(!Queue_empty(queue)) {
    UnionFindSet* current = Queue_dequeue(queue);
    if(UnionFindSet_same(current, info->visited_set)) {
      continue;
    }

    visit(UnionFindSet_get(current));
    UnionFindSet_union(current, info->visited_set);

    EdgeIterator* adjacents = Graph_adjacents(info->graph, UnionFindSet_get(current));
    for_each(AdjacentsEdge_it(adjacents), ^(void* obj) {
      EdgeInfo* ei = (EdgeInfo*) obj;
      void* neighbour = ei->destination;
      UnionFindSet* neighbour_set;

      if(!Dictionary_get(info->vertex_set, neighbour, (void**)&neighbour_set)) {
        Error_raise( Error_new( ERROR_GENERIC, "Cannot find searched vertex" ));
      }

      if(!UnionFindSet_same(neighbour_set, info->visited_set)) {
        Queue_enqueue(queue, neighbour_set);
      }
    });
  }

  Queue_free(queue);
}
