#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"
#include "graph_visiting.h"
#include "dijkstra.h"
#include "unit_testing.h"
#include "dictionary.h"
#include "double_container.h"
#include "macros.h"
#include "kruskal.h"
#include "iterator_functions.h"
#include "mem.h"
#include "prim.h"


static Graph* build_graph_fixtures() {
  KeyInfo* keys = KeyInfo_new(Key_string_compare, Key_string_hash);
  Graph* graph = Graph_new(keys);

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

static void free_graph_fixture(Graph* graph) {
  VertexIterator* v_it = Graph_vertices(graph);
  while(!VertexIterator_end(v_it)) {
    void* vertex = VertexIterator_get(v_it);

    EdgeIterator* e_it = Graph_adjacents(graph, vertex);
    while(!EdgeIterator_end(e_it)) {
      DoubleContainer_free((DoubleContainer*) EdgeIterator_get(e_it)->info);
      EdgeIterator_next(e_it);
    }
    EdgeIterator_free(e_it);

    VertexIterator_next(v_it);
  }
  VertexIterator_free(v_it);

  KeyInfo* keyInfo = Graph_keyInfo(graph);
  Graph_free(graph);
  KeyInfo_free(keyInfo);
}



static void test_graph_add_vertex() {
  Graph* graph = build_graph_fixtures();
  assert_equal( 6l, (long) Graph_size(graph) );

  Graph_add_vertex(graph, "v7");
  assert_equal( 7l, (long) Graph_size(graph));

  free_graph_fixture(graph);
}

static void test_graph_add_edge() {
  Graph* graph = build_graph_fixtures();
  Graph_add_edge(graph, "v2", "v5", DoubleContainer_new(4.0));
  EdgeIterator* e_it = Graph_adjacents(graph, "v2");

  int found = FALSE;
  while(!found && !EdgeIterator_end(e_it)) {
    if(!strcmp(EdgeIterator_get(e_it)->destination, "v5")) {
      found = TRUE;
    }
    EdgeIterator_next(e_it);
  }
  EdgeIterator_free(e_it);

  assert(found == TRUE);
  free_graph_fixture(graph);
}

static void test_graph_vertices() {
  Graph* graph = build_graph_fixtures();
  VertexIterator* v_it = Graph_vertices(graph);
  int count = 0;
  while(!VertexIterator_end(v_it)) {
    count+=1;
    VertexIterator_next(v_it);
  }
  VertexIterator_free(v_it);

  assert_equal( (long) count, 6l );
  free_graph_fixture(graph);
}

static void test_graph_adjacencts() {
  Graph* graph = build_graph_fixtures();
  EdgeIterator* e_it = Graph_adjacents(graph, "v1");
  int count = 0;
  while(!EdgeIterator_end(e_it)) {
    count+=1;
    EdgeIterator_next(e_it);
  }
  EdgeIterator_free(e_it);

  assert_equal( (long) count, 2l );
  free_graph_fixture(graph);
}

static void test_dijkstra() {
  Graph* graph = build_graph_fixtures();
  double (*info_to_double)(const void*);
  info_to_double = (double (*)(const void*)) DoubleContainer_get;

  Dijkstra* d = Dijkstra_new(graph,info_to_double);
  void** min_path = Dijkstra_minpath(d, "v1", "v6");
  assert(min_path!=NULL);
  Dijkstra_free(d);

  int index = 0;
  while(min_path[index]!=NULL) {
    ++index;
  }
  assert_equal( (long) index, 4l );
  assert_string_equal(min_path[0], "v1");
  assert_string_equal(min_path[1], "v4");
  assert_string_equal(min_path[2], "v5");
  assert_string_equal(min_path[3], "v6");

  Mem_free(min_path);
  free_graph_fixture(graph);
}


static void test_kruskal() {
  Graph* graph = build_graph_fixtures();
  Graph_add_vertex(graph, "v8");
  Graph_add_vertex(graph, "v9");

  Graph_add_edge(graph, "v8", "v9", DoubleContainer_new(3.0));

  double (*info_to_double)(const void*);
  info_to_double = (double (*)(const void*)) DoubleContainer_get;

  Kruskal* k = Kruskal_new(graph, info_to_double);
  Graph* mintree = Kruskal_mintree(k);
  __block double treesize = 0.0;

  for_each(Edge_it(mintree), ^(void* obj) {
    EdgeInfo* ei = obj;
    treesize += DoubleContainer_get(ei->info);
  });

  assert_equal32( (int)treesize/2, 8);

  Kruskal_free(k);
  Graph_free(mintree);
  free_graph_fixture(graph);
}

static void test_prim() {
  Graph* graph = build_graph_fixtures();

  Graph_add_edge(graph, "v2", "v1", DoubleContainer_new(1.0));
  Graph_add_edge(graph, "v4", "v1", DoubleContainer_new(3.0));
  Graph_add_edge(graph, "v3", "v2", DoubleContainer_new(4.0));
  Graph_add_edge(graph, "v2", "v4", DoubleContainer_new(1.0));
  Graph_add_edge(graph, "v5", "v3", DoubleContainer_new(1.0));
  Graph_add_edge(graph, "v5", "v4", DoubleContainer_new(0.0));
  Graph_add_edge(graph, "v6", "v4", DoubleContainer_new(3.0));
  Graph_add_edge(graph, "v6", "v5", DoubleContainer_new(2.0));

  Graph_add_vertex(graph, "v8");
  Graph_add_vertex(graph, "v9");

  Graph_add_edge(graph, "v8", "v9", DoubleContainer_new(3.0));
  Graph_add_edge(graph, "v9", "v8", DoubleContainer_new(3.0));

  double (*info_to_double)(const void*);
  info_to_double = (double (*)(const void*)) DoubleContainer_get;

  Prim* k = Prim_new(graph, info_to_double);
  Graph* mintree = Prim_mintree(k, "v1");
  __block double treesize = 0.0;

  for_each(Edge_it(mintree), ^(void* obj) {
    EdgeInfo* ei = obj;
    treesize += DoubleContainer_get(ei->info);
  });

  assert_equal32( (int)treesize/2, 8);

  Prim_free(k);
  Graph_free(mintree);
  free_graph_fixture(graph);
}

static void test_graph_foreach_edge() {
  Graph* graph = build_graph_fixtures();
  __block size_t count = 0;

  for_each( Edge_it(graph), ^(void* obj) {
    EdgeInfo* info = (EdgeInfo*) obj;
    assert_not_null(info->source);
    assert_not_null(info->destination);
    assert_not_null(info->info);
    count+=1;
  });

  assert_equal(8l, count);
  free_graph_fixture(graph);
}

static void test_graph_foreach_vertex() {
  Graph* graph = build_graph_fixtures();
  __block size_t count = 0;
  for_each(Vertex_it(graph), ^(void* vertex) {
    assert_not_null(vertex);
    count += 1;
  });

  assert_equal(6l, count);
  free_graph_fixture(graph);
}

static void test_iterate_over_unconnected_graph() {
  KeyInfo* keys = KeyInfo_new(Key_string_compare, Key_string_hash);
  Graph* graph = Graph_new(keys);

  Graph_add_vertex(graph, "v1");
  Graph_add_vertex(graph, "v2");
  Graph_add_vertex(graph, "v3");
  Graph_add_vertex(graph, "v4");
  Graph_add_vertex(graph, "v5");
  Graph_add_vertex(graph, "v6");

  for_each(Edge_it(graph), ^(UNUSED(void* info)) {
    assert_not_reached();
  });

  free_graph_fixture(graph);
}


// dfs visit on:
// v1->v2->v3->(v1)
// v4
// v5->v6
static void test_dfs_visit() {
  KeyInfo* keys = KeyInfo_new(Key_string_compare, Key_string_hash);
  Graph* graph = Graph_new(keys);

  Graph_add_vertex(graph, "v1");
  Graph_add_vertex(graph, "v2");
  Graph_add_vertex(graph, "v3");
  Graph_add_vertex(graph, "v4");
  Graph_add_vertex(graph, "v5");
  Graph_add_vertex(graph, "v6");

  Graph_add_edge(graph, "v1", "v2", NULL);
  Graph_add_edge(graph, "v2", "v3", NULL);
  Graph_add_edge(graph, "v3", "v1", NULL);

  Graph_add_edge(graph, "v5", "v6", NULL);

  void* vertex;
  VisitingInfo* info = VisitingInfo_new(graph);
  __block size_t visited_vertices_count = 0;
  size_t num_components = 0;
  while( (vertex = VisitingInfo_next_unvisited(info)) != NULL ) {
    num_components += 1;
    Graph_depth_first_visit(info, vertex, ^(void* UNUSED(v)) {
      visited_vertices_count += 1;
    });
  }

  VisitingInfo_free(info);
  assert_equal(3l, num_components);
  assert_equal(6l, visited_vertices_count);

  Graph_free(graph);
  KeyInfo_free(keys);
}


// bf visit on:
// v1->v2->v3->(v1)
// v4
// v5->v6
static void test_bfs_visit() {
  KeyInfo* keys = KeyInfo_new(Key_string_compare, Key_string_hash);
  Graph* graph = Graph_new(keys);

  Graph_add_vertex(graph, "v1");
  Graph_add_vertex(graph, "v2");
  Graph_add_vertex(graph, "v3");
  Graph_add_vertex(graph, "v4");
  Graph_add_vertex(graph, "v5");
  Graph_add_vertex(graph, "v6");

  Graph_add_edge(graph, "v1", "v2", NULL);
  Graph_add_edge(graph, "v2", "v3", NULL);
  Graph_add_edge(graph, "v3", "v1", NULL);

  Graph_add_edge(graph, "v5", "v6", NULL);

  void* vertex;
  VisitingInfo* info = VisitingInfo_new(graph);
  __block size_t visited_vertices_count = 0;
  size_t num_components = 0;
  while( (vertex = VisitingInfo_next_unvisited(info)) != NULL ) {
    num_components += 1;
    Graph_breadth_first_visit(info, vertex, ^(void* UNUSED(v)) {
      visited_vertices_count += 1;
    });
  }
  VisitingInfo_free(info);

  assert_equal(3l, num_components);
  assert_equal(6l, visited_vertices_count);

  Graph_free(graph);
  KeyInfo_free(keys);
}

int main() {
  start_tests("graph");
  test(test_graph_add_vertex);
  test(test_graph_add_edge);
  test(test_graph_vertices);
  test(test_graph_adjacencts);
  test(test_iterate_over_unconnected_graph);
  end_tests();

  start_tests("iterators");
  test(test_graph_foreach_vertex);
  test(test_graph_foreach_edge);
  end_tests();

  start_tests("dijkstra");
  test(test_dijkstra);
  end_tests();

  start_tests("kruskal");
  test(test_kruskal);
  end_tests();

  start_tests("prim");
  test(test_prim);
  end_tests();


  start_tests("visits");
  test(test_dfs_visit);
  test(test_bfs_visit);
  end_tests();

  return 0;
}
