#include <stdlib.h>
#include <stdio.h>

#include "dijkstra.h"
#include "dictionary.h"
#include "priority_queue.h"

typedef struct _DoubleContainer {
  double value;
}* DoubleContainer;

DoubleContainer DoubleContainer_new(double d) {
  DoubleContainer result = (DoubleContainer) malloc(sizeof(struct _DoubleContainer));
  result->value = d;
  return result;
}

double DoubleContainer_get(DoubleContainer dc) {
  return dc->value;
}

unsigned int path_len(Dictionary parents, void* dest) {
  unsigned int count = 0;
  void* current = dest;
  void* next;
  if(Dictionary_get(parents, dest, &next) == 0) {
    printf("Error in retrieving parents");
    exit(1);
  }
  while(current != next) {
    count += 1;

    if(Dictionary_get(parents, dest, &next) == 0) {
      printf("Error in retrieving parents");
      exit(1);
    }
  }

  return count;
}

void** build_path(Dictionary parents, void* dest) {
  unsigned int len = path_len(parents, dest);
  void** result = (void**) malloc(sizeof(void*)*(len+1));
  result[len] = NULL;

  void* current = dest;
  void* next;
  Dictionary_get(parents, current, &next);
  int last = len - 1;

  while(current!=next) {
    result[last] = current;
    current = next;
    Dictionary_get(parents, current, &next);
    last -= 1;
  }

  return result;
}


// FIXME: MEMORY DEALLOC!
void** dijkstra(Graph graph, void* source, void* dest, double (*graph_info_to_double)(void*)) {
  KIComparator comparator = KeyInfo_comparator(Graph_keyInfo(graph));
  PriorityQueue pq = PriorityQueue_new();
  Dictionary parents = Dictionary_new(Graph_keyInfo(graph));
  Dictionary distances = Dictionary_new(Graph_keyInfo(graph));

  Dictionary_set(parents, source, source);
  Dictionary_set(distances, source, DoubleContainer_new(0.0));

  PriorityQueue_push(pq, source, 0.0);
  while(!PriorityQueue_empty(pq)) {
    void* current = PriorityQueue_top_value(pq);
    double current_dist = PriorityQueue_top_priority(pq);
    PriorityQueue_pop(pq);

    if(comparator(source, dest) == 0) {
      return build_path(dest, parents);
    }

    EdgeIterator neighbours = Graph_adjacents(graph, current);
    while(!EdgeIterator_end(neighbours)) {
      EdgeInfo edge = EdgeIterator_get(neighbours);
      void* child = edge.vertex;
      double edge_distance = graph_info_to_double(edge.info);

      double new_distance = current_dist + edge_distance;
      DoubleContainer child_distance;
      int child_found = Dictionary_get(distances, child, (void**)&child_distance);
      if(!child_found || DoubleContainer_get(child_distance) <= new_distance ) {
        continue;
      }

      Dictionary_set(parents, child, current);
      if(!child_found) {
        PriorityQueue_push(pq, child, new_distance);
      } else {
        PriorityQueue_decrease_priority(pq, child, new_distance);
      }

      Dictionary_set(distances, child, DoubleContainer_new(new_distance));
    }
  }

  return NULL;
}
