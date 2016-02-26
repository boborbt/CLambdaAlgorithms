#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "dijkstra.h"
#include "priority_queue.h"
#include "unit_testing.h"
#include "dictionary.h"


PriorityQueue build_pq_fixtures() {
  PriorityQueue pq = PriorityQueue_new();

  PriorityQueue_push(pq, "10", 10.0);
  PriorityQueue_push(pq, "2", 2.0);
  PriorityQueue_push(pq, "4", 4.0);
  PriorityQueue_push(pq, "3", 3.0);
  PriorityQueue_push(pq, "6", 6.0);

  return pq;
}


Graph build_graph_fixtures() {
  KeyInfo key_info = KeyInfo_new(Dictionary_string_compare, Dictionary_string_hash);
  Graph graph = Graph_new(key_info);

  Graph_add_vertex(graph, "v1");
  Graph_add_vertex(graph, "v2");
  Graph_add_vertex(graph, "v3");
  Graph_add_vertex(graph, "v4");
  Graph_add_vertex(graph, "v5");
  Graph_add_vertex(graph, "v6");

  Graph_add_edge(graph, "v1", "v2", DoubleContainer_new(1.0));
  Graph_add_edge(graph, "v1", "v4", DoubleContainer_new(3.0));
  Graph_add_edge(graph, "v2", "v3", DoubleContainer_new(4.0));
  Graph_add_edge(graph, "v4", "v2", DoubleContainer_new(1.0));
  Graph_add_edge(graph, "v3", "v5", DoubleContainer_new(1.0));
  Graph_add_edge(graph, "v4", "v5", DoubleContainer_new(0.0));
  Graph_add_edge(graph, "v4", "v6", DoubleContainer_new(3.0));
  Graph_add_edge(graph, "v5", "v6", DoubleContainer_new(2.0));

  return graph;
}

void free_graph_fixture(Graph graph) {
  VertexIterator v_it = Graph_vertices(graph);
  while(!VertexIterator_end(v_it)) {
    const void* vertex = VertexIterator_get(v_it);

    EdgeIterator e_it = Graph_adjacents(graph, vertex);
    while(!EdgeIterator_end(e_it)) {
      DoubleContainer_free((DoubleContainer) EdgeIterator_get(e_it).info);
      EdgeIterator_next(e_it);
    }
    EdgeIterator_free(e_it);

    VertexIterator_next(v_it);
  }
  VertexIterator_free(v_it);

  Graph_free(graph);
}

void test_priority_queue_creation() {
  PriorityQueue pq = PriorityQueue_new();
  assert_equal( 0l, (long) PriorityQueue_size(pq) );
  assert(PriorityQueue_empty(pq));
  PriorityQueue_free(pq);
}

void test_priority_queue_decrease_priority() {
  PriorityQueue pq = build_pq_fixtures();
  PriorityQueue_decrease_priority(pq, "10", 1.0);
  assert_string_equal(PriorityQueue_top_value(pq), "10");
  PriorityQueue_free(pq);
}

void test_priority_queue_push_min_element() {
  PriorityQueue pq = build_pq_fixtures();
  PriorityQueue_push(pq, "test", 1.0);
  assert_string_equal((char*) PriorityQueue_top_value(pq), "test");
  assert_double_equal(PriorityQueue_top_priority(pq), 1.0);
  PriorityQueue_free(pq);
}

void test_priority_queue_top_value() {
  PriorityQueue pq = build_pq_fixtures();
  assert_string_equal((char*)PriorityQueue_top_value(pq), "2");
  PriorityQueue_free(pq);
}

void test_priority_queue_top_priority() {
  PriorityQueue pq = build_pq_fixtures();
  assert_double_equal(PriorityQueue_top_priority(pq), 2.0);
  PriorityQueue_free(pq);
}

void test_priority_queue_pop() {
  PriorityQueue pq = build_pq_fixtures();
  assert_double_equal(PriorityQueue_top_priority(pq), 2.0);

  assert_equal(5l, (long)PriorityQueue_size(pq));
  PriorityQueue_pop(pq);

  assert_equal(4l, (long) PriorityQueue_size(pq));
  assert(PriorityQueue_top_priority(pq) == 3.0);

  PriorityQueue_free(pq);
}

void test_priority_queue_pop_order() {
  PriorityQueue pq = build_pq_fixtures();
  double priority_order[] = {2.0, 3.0, 4.0, 6.0, 10.0};
  int count = 0;
  while(!PriorityQueue_empty(pq) && count < 5) {
    assert_double_equal( PriorityQueue_top_priority(pq), priority_order[count++]);
    PriorityQueue_pop(pq);
  }
  PriorityQueue_free(pq);
}

void test_graph_add_vertex() {
  Graph graph = build_graph_fixtures();
  assert_equal( 6l, (long) Graph_size(graph) );

  Graph_add_vertex(graph, "v7");
  assert_equal( 7l, (long) Graph_size(graph));

  free_graph_fixture(graph);
}

void test_graph_add_edge() {
  Graph graph = build_graph_fixtures();
  Graph_add_edge(graph, "v2", "v5", DoubleContainer_new(4.0));
  EdgeIterator e_it = Graph_adjacents(graph, "v2");

  int found = FALSE;
  while(!found && !EdgeIterator_end(e_it)) {
    if(!strcmp(EdgeIterator_get(e_it).vertex, "v5")) {
      found = TRUE;
    }
    EdgeIterator_next(e_it);
  }
  EdgeIterator_free(e_it);

  assert(found == TRUE);
  free_graph_fixture(graph);
}

void test_graph_vertices() {
  Graph graph = build_graph_fixtures();
  VertexIterator v_it = Graph_vertices(graph);
  int count = 0;
  while(!VertexIterator_end(v_it)) {
    count+=1;
    VertexIterator_next(v_it);
  }
  VertexIterator_free(v_it);

  assert_equal( (long) count, 6l );
  free_graph_fixture(graph);
}

void test_graph_adjacencts() {
  Graph graph = build_graph_fixtures();
  EdgeIterator e_it = Graph_adjacents(graph, "v1");
  int count = 0;
  while(!EdgeIterator_end(e_it)) {
    count+=1;
    EdgeIterator_next(e_it);
  }
  EdgeIterator_free(e_it);

  assert_equal( (long) count, 2l );
  free_graph_fixture(graph);
}

void test_dijkstra() {
  Graph graph = build_graph_fixtures();
  double (*info_to_double)(const void*);
  info_to_double = (double (*)(const void*)) DoubleContainer_get;

  const void** min_path = dijkstra(graph, "v1", "v6",  info_to_double);
  assert(min_path!=NULL);

  int index = 0;
  while(min_path[index]!=NULL) {
    ++index;
  }
  assert_equal( (long) index, 4l );
  assert_string_equal(min_path[0], "v1");
  assert_string_equal(min_path[1], "v4");
  assert_string_equal(min_path[2], "v5");
  assert_string_equal(min_path[3], "v6");

  free(min_path);
  free_graph_fixture(graph);
}



int main(int argc, char const *argv[]) {
  test_priority_queue_creation();
  test_priority_queue_push_min_element();
  test_priority_queue_top_value();
  test_priority_queue_top_priority();
  test_priority_queue_pop();
  test_priority_queue_pop_order();
  test_priority_queue_decrease_priority();

  test_graph_add_vertex();
  test_graph_add_edge();
  test_graph_vertices();
  test_graph_adjacencts();

  test_dijkstra();

  return 0;
}
