#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "dijkstra.h"
#include "dictionary.h"
#include "priority_queue.h"
#include "double_container.h"


struct _Dijkstra {
  Graph graph;
  double (*graph_info_to_double)(const void*);
  KIComparator comparator;
  PriorityQueue pq;
  Dictionary parents;
  Dictionary distances;
};

static unsigned int path_len(Dictionary parents, const void* dest) {
  unsigned int count = 1;
  const void* current = dest;
  void* next;
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

static void** build_path(Dictionary parents, void* dest) {
  unsigned int len = path_len(parents, dest);
  void** result = (void**) malloc(sizeof(void*)*(len+1));
  result[len] = NULL;

  void* current = dest;
  void* next;
  Dictionary_get(parents, current, &next);
  unsigned int last = len - 1;

  while(last != (unsigned int)-1) {
    result[last] = current;
    current = next;
    Dictionary_get(parents, current, &next);
    last -= 1;
  }

  return result;
}

static void cleanup_distances_values(Dictionary distances) {
  DictionaryIterator it = DictionaryIterator_new(distances);
  while(!DictionaryIterator_end(it)) {
    Elem elem = DictionaryIterator_get(it);
    DoubleContainer_free((DoubleContainer) elem->value);
    DictionaryIterator_next(it);
  }
  DictionaryIterator_free(it);
}

static void** cleanup_and_build_path(Dijkstra state, void* dest) {
  void** result = NULL;
  if(dest != NULL) {
    result = build_path(state->parents, dest);
  }

  PriorityQueue_free(state->pq);
  Dictionary_free(state->parents);
  cleanup_distances_values(state->distances);
  Dictionary_free(state->distances);

  return result;
}


Dijkstra Dijkstra_new(Graph graph, double (*graph_info_to_double)(const void*)) {
  Dijkstra result = (Dijkstra) malloc(sizeof(struct _Dijkstra));

  result->graph = graph;
  result->graph_info_to_double = graph_info_to_double;

  return result;
}

void Dijkstra_free(Dijkstra d) {
  free(d);
}

static void Dijkstra_init_state(Dijkstra state) {
  state->comparator = KeyInfo_comparator(Graph_keyInfo(state->graph));
  state->pq = PriorityQueue_new();
  state->parents = Dictionary_new(Graph_keyInfo(state->graph));
  state->distances = Dictionary_new(Graph_keyInfo(state->graph));
}


static void examine_neighbours(Dijkstra state, void* current, double current_dist) {
  EdgeIterator neighbours = Graph_adjacents(state->graph, current);
  for(;!EdgeIterator_end(neighbours); EdgeIterator_next(neighbours)) {
    EdgeInfo edge = EdgeIterator_get(neighbours);
    void* child = edge.vertex;
    double edge_distance = state->graph_info_to_double(edge.info);

    double new_distance = current_dist + edge_distance;
    DoubleContainer child_distance;
    int child_found = Dictionary_get(state->distances, child, (void **)&child_distance);
    if(child_found && DoubleContainer_get(child_distance) <= new_distance ) {
      continue;
    }

    Dictionary_set(state->parents, child, current);
    if(!child_found) {
      PriorityQueue_push(state->pq, child, new_distance);
    } else {
      PriorityQueue_decrease_priority(state->pq, child, new_distance);
    }

    void* old_value = NULL;
    if(Dictionary_get(state->distances, child, (void **)&old_value)) {
      DoubleContainer_free((DoubleContainer) old_value);
    }
    Dictionary_set(state->distances, child, DoubleContainer_new(new_distance));
  }
  EdgeIterator_free(neighbours);
}

void** Dijkstra_minpath(Dijkstra state, void* source, void* dest) {
  Dijkstra_init_state(state);

  Dictionary_set(state->parents, source, source);
  Dictionary_set(state->distances, source, DoubleContainer_new(0.0));

  PriorityQueue_push(state->pq, source, 0.0);
  while(!PriorityQueue_empty(state->pq)) {
    void* current = PriorityQueue_top_value(state->pq);
    double current_dist = PriorityQueue_top_priority(state->pq);
    PriorityQueue_pop(state->pq);

    if(state->comparator(current, dest) == 0) {
      return cleanup_and_build_path(state, dest );
    }

    examine_neighbours(state, current, current_dist);
  }

  return cleanup_and_build_path(state, NULL);
}
