#include <float.h>
#include <stdio.h>
#include <math.h>

#include "prim.h"
#include "priority_queue.h"
#include "iterator_functions.h"
#include "graph.h"
#include "mem.h"


// Opaque type to support the Dijkstra* algorithm
struct _Prim {
  Graph* graph;
  double (*graph_info_to_double)(const void*);
};

// Creates and initializes the data structures used by the Prim* algorithm
Prim* Prim_new(Graph* graph, double (*graph_info_to_double)(const void*)) {
  Prim* prim = (Prim*) Mem_alloc(sizeof(Prim));
  prim->graph = graph;
  prim->graph_info_to_double = graph_info_to_double;

  return prim;
}

// Runs Prim* and returns the found minimal spanning tree
// The minimal path is returned as a newly alloced graph having the same
// KeyInfo* as the source graph
// The user needs to dealloc the graph when he is finished with it.
Graph* Prim_mintree(Prim* prim, void* root) {
  PriorityQueue* pq = PriorityQueue_new(PQOrder_ascending);
  Graph* result = Graph_new(Graph_keyInfo(prim->graph));

  Graph_add_vertex(result, root);

  PriorityQueue_push(pq, root, 0.0);
  for_each(Vertex_it(prim->graph), ^(void* obj) {
    if(obj != root) {
      PriorityQueue_push(pq, obj, DBL_MAX);
    }
  });


  while( !PriorityQueue_empty(pq) && fabs(PriorityQueue_top_priority(pq) - DBL_MAX) > 0.0001) {
    void* candidate = PriorityQueue_top_value(pq);
    PriorityQueue_pop(pq);

    EdgeIterator* adjacents = Graph_adjacents(prim->graph, candidate);
    for_each( AdjacentsEdge_it(adjacents), ^(void* info) {
      EdgeInfo* ei = (EdgeInfo*) info;

      if(!Graph_has_vertex(result, ei->destination)) {
        int success = PriorityQueue_try_decrease_priority(pq, ei->destination, prim->graph_info_to_double(ei->info));

        if(success) {
          Graph_add_vertex(result, ei->destination);
          Graph_add_edge(result, candidate, ei->destination, ei->info);
          Graph_add_edge(result, ei->destination, candidate, ei->info);
        }
      }
    });
  }

  PriorityQueue_free(pq);

  return result;
}

// Frees the datastructures used by the Prim* algorithm
void Prim_free(Prim* prim) {
  Mem_free(prim);
}
