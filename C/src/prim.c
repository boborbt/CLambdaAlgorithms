#include <float.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "prim.h"
#include "priority_queue.h"
#include "iterator_functions.h"
#include "graph.h"
#include "dictionary.h"
#include "mem.h"


// Opaque type to support the Dijkstra* algorithm
struct _Prim {
  Graph* graph;
  Dictionary* parents;
  PriorityQueue* pq;
  double (*graph_info_to_double)(const void*);
};

// Creates and initializes the data structures used by the Prim* algorithm
Prim* Prim_new(Graph* graph, double (*graph_info_to_double)(const void*)) {
  Prim* prim = (Prim*) Mem_alloc(sizeof(Prim));
  prim->graph = graph;
  prim->parents = Dictionary_new(Graph_keyInfo(graph));
  prim->graph_info_to_double = graph_info_to_double;
  prim->pq = PriorityQueue_new(PQOrder_ascending);

  return prim;
}

static void Prim_init(Prim* prim) {
  for_each(Vertex_it(prim->graph), ^(void* node) {
    Dictionary_set(prim->parents, node, NULL);
    PriorityQueue_push(prim->pq, node, DBL_MAX);
  });
}

static Graph* Prim_build_result(Prim* prim) {
  Graph* result = Graph_new(Graph_keyInfo(prim->graph));

  for_each(Dictionary_it(prim->parents), ^(void* elem) {
    KeyValue* kv = (KeyValue*) elem;

    if(kv->value == NULL) {
      return;
    }

    if(!Graph_has_vertex(result, kv->key)) {
      Graph_add_vertex(result, kv->key);
    }

    if(!Graph_has_vertex(result, kv->value)) {
      Graph_add_vertex(result, kv->value);
    }

    void* edge_info = Graph_edge_info(prim->graph, kv->key, kv->value);
    Graph_add_edge(result, kv->key, kv->value, edge_info);
    Graph_add_edge(result, kv->value, kv->key, edge_info);
  });

  return result;
}

// Runs Prim* and returns the found minimal spanning tree
// The minimal path is returned as a newly alloced graph having the same
// KeyInfo* as the source graph
// The user needs to dealloc the graph when he is finished with it.
Graph* Prim_mintree(Prim* prim, void* root) {
  Prim_init(prim);
  PriorityQueue_try_decrease_priority(prim->pq, root, 0.0);

  while( !PriorityQueue_empty(prim->pq)) {
    void* candidate = PriorityQueue_top_value(prim->pq);
    PriorityQueue_pop(prim->pq);

    EdgeIterator* adjacents = Graph_adjacents(prim->graph, candidate);
    for_each( AdjacentsEdge_it(adjacents), ^(void* info) {
      EdgeInfo* ei = (EdgeInfo*) info;

      double old_weight;
      double new_weight = prim->graph_info_to_double(ei->info);

      int found_in_queue = PriorityQueue_get_priority(prim->pq, ei->destination, &old_weight);
      if( found_in_queue &&  new_weight < old_weight ) {
        Dictionary_set(prim->parents, ei->destination, candidate);
        PriorityQueue_decrease_priority(prim->pq, ei->destination, new_weight);
      }
    });
  }

   Graph* result = Prim_build_result(prim);
   return result;
}

// Frees the datastructures used by the Prim* algorithm
void Prim_free(Prim* prim) {
  Dictionary_free(prim->parents);
  PriorityQueue_free(prim->pq);
  Mem_free(prim);
}
