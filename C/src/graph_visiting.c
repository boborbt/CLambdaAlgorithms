#include "graph_visiting.h"
#include "dictionary.h"
#include "union_find.h"
#include <stdlib.h>
#include "errors.h"

struct _VisitingInfo {
  Graph graph;
  void (*visit)(void*, void*);
  void* user_info;

  Dictionary vertex_set;
  UnionFindSet visited_set;
  DictionaryIterator current;
};


static void VisitingInfo_init_singletons(VisitingInfo info)  {
  foreach_graph_vertex(info->graph, ^(void* vertex){
    Dictionary_set(info->vertex_set, vertex, UnionFindSet_new(vertex));
  });
}

VisitingInfo VisitingInfo_new(Graph graph, void (*visit)(void*, void*), void* user_info) {
  VisitingInfo result = (VisitingInfo) malloc(sizeof(struct _VisitingInfo));

  result->graph = graph;
  result->visit = visit;
  result->vertex_set = Dictionary_new(Graph_keyInfo(graph));
  result->user_info = user_info;
  VisitingInfo_init_singletons(result);
  result->current = DictionaryIterator_new(result->vertex_set);
  result->visited_set = DictionaryIterator_get(result->current)->value;

  return result;
}

void* VINode_get(VINode node) {
  return UnionFindSet_get(node);
}

void VisitingInfo_free(VisitingInfo info) {
  foreach_dictionary_key_value(info->vertex_set, ^(KeyValue* kv) {
    UnionFindSet_free(kv->value);
  });

  free(info);
}

VINode VisitingInfo_next_unvisited(VisitingInfo info) {
  if(DictionaryIterator_end(info->current)) {
    return NULL;
  }

  VINode result = DictionaryIterator_get(info->current)->value;
  DictionaryIterator_next(info->current);

  return result;
}


void Graph_depth_first_visit(VisitingInfo info, VINode source) {
  UnionFindSet_union(source, info->visited_set);
  info->visit(VINode_get(source), info->user_info);

  foreach_graph_edge_from_iterator(Graph_adjacents(info->graph, VINode_get(source)), ^(EdgeInfo ei) {
    void* neighbour = ei.vertex;
    VINode neighbour_set;

    if(!Dictionary_get(info->vertex_set, neighbour, &neighbour_set)) {
      Error_raise( Error_new( ERROR_GENERIC, "Cannot find searched vertex" ));
    }

    if(!UnionFindSet_same(neighbour_set, info->visited_set)) {
      Graph_depth_first_visit(info, neighbour_set);
    }
  });
}
