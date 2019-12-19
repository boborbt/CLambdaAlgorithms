#include <string.h>
#include <stdio.h>
#include "graph.h"
#include "iterator_functions.h"
#include "dictionary.h"
#include "basic_iterators.h"
#include "mem.h"


#define BUF_SIZE 1024

static void add_edge(Graph* graph, Dictionary* known_vertices,  const char* v1, const char* v2, const char* len) {
  void* vertex1 = NULL;
  void* vertex2 = NULL;

  if(!Dictionary_get(known_vertices, v1, &vertex1)) {
    vertex1 = Mem_strdup(v1);
    Dictionary_set(known_vertices, vertex1, vertex1);
    Graph_add_vertex(graph, vertex1);
  }

  if(!Dictionary_get(known_vertices, v2, &vertex2)) {
    vertex2 = Mem_strdup(v2);
    Dictionary_set(known_vertices, vertex2, vertex2);
    Graph_add_vertex(graph, vertex2);
  }

  if(!Graph_has_edge(graph, vertex1, vertex2)) {
    Graph_add_edge(graph, vertex1, vertex2, Mem_strdup(len));
    Graph_add_edge(graph, vertex2, vertex1, Mem_strdup(len));
  }
}

static Graph* load_graph(const char* filename) {
  KeyInfo* keyInfo = KeyInfo_new( Key_string_compare, Key_string_hash );
  Dictionary* known_vertices = Dictionary_new(keyInfo);
  Graph* graph = Graph_new(keyInfo);

  __block int count = 0;

  for_each(TextFile_it(filename, '\n'), ^(void* obj) {
    char buf1[BUF_SIZE];
    char buf2[BUF_SIZE];
    char buf3[BUF_SIZE];

    sscanf((char*) obj, "%[^,],%[^,],%s\n", buf1, buf2, buf3);
    add_edge(graph, known_vertices, buf1, buf2, buf3);
    count += 2;
  });

  printf("dc creation count:%d\n", count);
  Dictionary_free(known_vertices);
  return graph;
}


static void dot_graph(Graph* graph) {
    printf("digraph {\n");
    for_each(Vertex_it(graph), ^(void* vertex){
        printf("%s;\n", vertex);
    });

    for_each(Edge_it(graph), ^(void* obj) {
        EdgeInfo* info = obj;
        printf("%s -> %s [label = \"%s\"];\n", info->source, info->destination, info->info);
    });
    printf("}\n");
}



int main() {
    KeyInfo* ki = KeyInfo_new( Key_string_compare, Key_string_hash );
    Graph* graph = load_graph("italian_dist_graph.csv");

    dot_graph(graph);

    Graph_free(graph);
    KeyInfo_free(ki);
    return 0;
}
