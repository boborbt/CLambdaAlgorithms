#include "kruskal.h"
#include "array.h"
#include "dictionary.h"
#include "union_find.h"
#include "quick_sort.h"
#include "errors.h"
#include <stdio.h>
#include <assert.h>


struct _Kruskal {
  Graph graph;
  double (*graph_info_to_double)(const void*);
  Dictionary sets;
};

typedef struct _KruskalEdge {
  UnionFindSet source;
  UnionFindSet destination;
  void* info;
  double weight;
}* KruskalEdge;

typedef const struct _KruskalEdge* ConstKruskalEdge;

// Creates and initializes the data structures used by the Dijkstra algorithm
Kruskal Kruskal_new(Graph graph, double (*graph_info_to_double)(const void*)) {
  Kruskal result = (Kruskal) malloc(sizeof(struct _Kruskal));
  result->graph = graph;
  result->graph_info_to_double = graph_info_to_double;
  result->sets = Dictionary_new(Graph_keyInfo(graph));

  return result;
}

static UnionFindSet KruskalEdge_source(const ConstKruskalEdge edge) {
  return edge->source;
}

static UnionFindSet KruskalEdge_destination(const ConstKruskalEdge edge) {
  return edge->destination;
}

static double KruskalEdge_weight(const ConstKruskalEdge edge) {
  return edge->weight;
}

static void* KruskalEdge_info(const ConstKruskalEdge edge) {
  return edge->info;
}

static KruskalEdge KruskalEdge_new(void* src, void* dst, double weight, void* info) {
  KruskalEdge result = (KruskalEdge) malloc(sizeof(struct _KruskalEdge));

  if(src == NULL || dst == NULL) {
    printf("DEBUG: src or dst are NULL!\n");
    exit(1);
  }

  result->source = src;
  result->destination = dst;
  result->weight = weight;
  result->info = info;

  return result;
}

static void KruskalEdge_free(KruskalEdge edge) {
  free(edge);
}

static void Kruskal_initializeSets(Kruskal k) {
  foreach_graph_vertex(k->graph, ^(void* vertex) {
    Dictionary_set(k->sets, vertex, UnionFindSet_new(vertex));
  });
}

static int Kruskal_compare_edges(const void* obj1, const void* obj2) {
  const KruskalEdge e1 = *(const KruskalEdge*) obj1;
  const KruskalEdge e2 = *(const KruskalEdge*) obj2;

  if( KruskalEdge_weight(e1) < KruskalEdge_weight(e2) ) {
    return -1;
  }

  if( KruskalEdge_weight(e1) > KruskalEdge_weight(e2) ) {
    return 1;
  }

  return 0;
}

static Array Kruskal_initEdgesArray(Kruskal k) {
  Array result =  Array_new(Graph_size(k->graph)*10, sizeof(KruskalEdge));

  foreach_graph_edge(k->graph, ^(void* src, void* dest, void* info){
    UnionFindSet set1 = NULL;
    UnionFindSet set2 = NULL;

    Dictionary_get(k->sets, src,  (void**) &set1);
    Dictionary_get(k->sets, dest, (void**) &set2);

    if(set1 == NULL || set2 == NULL) {
      Error_raise(Error_new(ERROR_GENERIC, "Cannot find vertices in Kruskal sets dictionary"));
    }

    KruskalEdge ke = KruskalEdge_new(set1, set2, k->graph_info_to_double(info), info );
    Array_add( result,  &ke);
  });

  Array_sort(result, Kruskal_compare_edges);

  return result;
}

// Runs Kruskal and returns the found minimal spanning tree
// The minimal path is returned as a newly alloced graph
// The user needs to dealloc the graph when finished with it.
// The returned graph uses the say keyinfo as the source graph
Graph Kruskal_mintree(Kruskal k) {
  KeyInfo ki = Graph_keyInfo(k->graph);

  Graph result = Graph_new(ki);
  foreach_graph_vertex(k->graph, ^(void* vertex) {
    Graph_add_vertex(result, vertex);
  });

  Kruskal_initializeSets(k);
  Array edges = Kruskal_initEdgesArray(k);

  for_each(Array_it(edges), ^(void* obj) {
    KruskalEdge edge = *(KruskalEdge*) obj;

    UnionFindSet source = KruskalEdge_source(edge);
    UnionFindSet dest = KruskalEdge_destination(edge);
    void* info = KruskalEdge_info(edge);

    if( !UnionFindSet_same(source,dest) ) {
      assert(!Graph_has_edge(result, UnionFindSet_get(source), UnionFindSet_get(dest)));
      assert(!Graph_has_edge(result, UnionFindSet_get(dest), UnionFindSet_get(source)));

      Graph_add_edge(result, UnionFindSet_get(source), UnionFindSet_get(dest), info);
      Graph_add_edge(result, UnionFindSet_get(dest), UnionFindSet_get(source), info);
      UnionFindSet_union(source, dest);
    }
  });

  for_each(Array_it(edges), ^(void* obj) {
    KruskalEdge ke = *(KruskalEdge*) obj;
    KruskalEdge_free(ke);
  });
  Array_free(edges);

  return result;
}

// Frees the datastructures used by the Kruskal algorithm
void Kruskal_free(Kruskal k) {
  for_each(Dictionary_it(k->sets),  ^(void* kv) {
    UnionFindSet_free(((KeyValue*)kv)->value);
  });

  Dictionary_free(k->sets);

  free(k);
}
