#include "graph_visiting.h"
#include "dictionary.h"
#include <stdlib.h>

struct _VisitingInfo {
  Graph graph;
  void (*visit)(void*, void*);
  void* user_info;
  Dictionary visited_vertices;
};

VisitingInfo VisitingInfo_new(Graph graph, void (*visit)(void*, void*), void* user_info) {
  VisitingInfo result = (VisitingInfo) malloc(sizeof(struct _VisitingInfo));

  result->graph = graph;
  result->visit = visit;
  result->visited_vertices = NULL;
  result->user_info = user_info;

  return result;
}

void VisitingInfo_free(VisitingInfo info) {
  free(info);
}

static void depth_first_visit(VisitingInfo info, void* source) {
  Dictionary_set(info->visited_vertices, source, "VISITED");
  info->visit(source, info->user_info);

  EdgeIterator adj_it = Graph_adjacents(info->graph, source);
  while(!EdgeIterator_end(adj_it)) {
    void* neighbour = EdgeIterator_get(adj_it).vertex;
    if(!Dictionary_get(info->visited_vertices, neighbour, NULL)) {
      depth_first_visit(info, neighbour);
    }
    EdgeIterator_next(adj_it);
  }
}

void Graph_depth_first_visit(VisitingInfo info, void* source) {
  info->visited_vertices = Dictionary_new(Graph_keyInfo(info->graph));
  depth_first_visit(info, source);
  Dictionary_free(info->visited_vertices);
  info->visited_vertices = NULL;
}
