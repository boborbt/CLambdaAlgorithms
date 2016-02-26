#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "dijkstra.h"
#include "dictionary.h"
#include "priority_queue.h"


typedef struct _DoubleContainer {
  double value;
}* DoubleContainer;

DoubleContainer DoubleContainer_new(double value) {
  DoubleContainer result = (DoubleContainer) malloc(sizeof(struct _DoubleContainer));
  result->value = value;
  return result;
}

void DoubleContainer_free(DoubleContainer dc) {
  free(dc);
}

double DoubleContainer_get(DoubleContainer dc) {
  return dc->value;
}

unsigned int path_len(Dictionary parents, const void* dest) {
  unsigned int count = 1;
  const void* current = dest;
  const void* next;
  if(Dictionary_get(parents, dest, &next) == 0) {
    printf("Error in retrieving parents");
    assert(0==1);
  }
  while(current != next) {
    count += 1;
    current = next;

    if(Dictionary_get(parents, current, &next) == 0) {
      printf("Error in retrieving parents");
      assert(0==1);
    }
  }

  return count;
}

const void** build_path(Dictionary parents, const void* dest) {
  unsigned int len = path_len(parents, dest);
  const void** result = (const void**) malloc(sizeof(void*)*(len+1));
  result[len] = NULL;

  const void* current = dest;
  const void* next;
  Dictionary_get(parents, current, &next);
  int last = len - 1;

  while(last>=0) {
    result[last] = current;
    current = next;
    Dictionary_get(parents, current, &next);
    last -= 1;
  }

  return result;
}

void cleanup_distances_values(Dictionary distances) {
  DictionaryIterator it = DictionaryIterator_new(distances);
  while(!DictionaryIterator_end(it)) {
    Elem elem = DictionaryIterator_get(it);
    DoubleContainer_free((DoubleContainer) elem->value);
    DictionaryIterator_next(it);
  }
  DictionaryIterator_free(it);
}

const void** cleanup_and_build_path(PriorityQueue pq, Dictionary parents, Dictionary distances, const void* dest) {
  const void** result = NULL;
  if(dest != NULL) {
    result = build_path(parents, dest);
  }

  PriorityQueue_free(pq);
  Dictionary_free(parents);
  cleanup_distances_values(distances);
  Dictionary_free(distances);

  return result;
}


const void** dijkstra(Graph graph, const void* source, const void* dest, double (*graph_info_to_double)(const void*)) {
  KIComparator comparator = KeyInfo_comparator(Graph_keyInfo(graph));
  PriorityQueue pq = PriorityQueue_new();
  Dictionary parents = Dictionary_new(Graph_keyInfo(graph));
  Dictionary distances = Dictionary_new(Graph_keyInfo(graph));

  Dictionary_set(parents, source, source);
  Dictionary_set(distances, source, DoubleContainer_new(0.0));

  PriorityQueue_push(pq, source, 0.0);
  while(!PriorityQueue_empty(pq)) {
    const void* current = PriorityQueue_top_value(pq);
    double current_dist = PriorityQueue_top_priority(pq);
    PriorityQueue_pop(pq);

    if(comparator(current, dest) == 0) {
      return cleanup_and_build_path(pq, parents, distances, dest );
    }

    EdgeIterator neighbours = Graph_adjacents(graph, current);
    for(;!EdgeIterator_end(neighbours); EdgeIterator_next(neighbours)) {
      EdgeInfo edge = EdgeIterator_get(neighbours);
      const void* child = edge.vertex;
      double edge_distance = graph_info_to_double(edge.info);

      double new_distance = current_dist + edge_distance;
      DoubleContainer child_distance;
      int child_found = Dictionary_get(distances, child, (const void**)&child_distance);
      if(child_found && DoubleContainer_get(child_distance) <= new_distance ) {
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
    EdgeIterator_free(neighbours);
  }

  return cleanup_and_build_path(pq, parents, distances, dest);
}
