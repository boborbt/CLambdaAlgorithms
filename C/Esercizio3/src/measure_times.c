#include "dijkstra.h"
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 1024


void add_edge(Graph graph, Dictionary known_vertices,  const char* v1, const char* v2, double len) {
  printf("inserting %s -> %s (%lf)\n", v1, v2, len);

  const void* vertex1 = NULL;
  const void* vertex2 = NULL;

  if(!Dictionary_get(known_vertices, v1, &vertex1)) {
    vertex1 = strdup(v1);
    Dictionary_set(known_vertices, vertex1, vertex1);
    Graph_add_vertex(graph, vertex1);
  }

  if(!Dictionary_get(known_vertices, v2, &vertex2)) {
    vertex2 = strdup(v2);
    Dictionary_set(known_vertices, vertex2, vertex2);
    Graph_add_vertex(graph, vertex2);
  }

  Graph_add_edge(graph, vertex1, vertex2, DoubleContainer_new(len));
}

Graph load_graph(const char* filename) {
  printf("Loading graph...\n");

  KeyInfo keyInfo = KeyInfo_new( Dictionary_string_compare, Dictionary_string_hash );
  Dictionary known_vertices = Dictionary_new(keyInfo);
  Graph graph = Graph_new(keyInfo);
  FILE* infile = fopen(filename, "r");

  while(!feof(infile)) {
    char buf1[BUF_SIZE];
    char buf2[BUF_SIZE];
    double len;

    fscanf(infile, "%[^,],%[^,],%lf\n", buf1, buf2, &len);
    add_edge(graph, known_vertices, buf1, buf2, len);
  }

  fclose(infile);

  return graph;
}

void print_path(Graph graph, const void** path) {
  if(path==NULL) {
    printf("Empty path!\n");
    return;
  }

  double path_len = 0.0;
  int index = 0;
  const void* current = path[index++];
  const void* prev = NULL;
  while(current!=NULL) {
    prev = current;
    current = path[index++];
    if(current!=NULL) {
      double len = DoubleContainer_get((DoubleContainer)Graph_edge_info(graph, prev, current));
      printf("%20s %20s %8.2lfKm\n", (char*) prev, current, len/1000.0);
      path_len += len;
    }
  }

  printf("\nLen: %8.2lfKm", path_len/1000);
}


int main(int argc, char const *argv[]) {
  Graph graph = load_graph(argv[1]);
  const void** min_path = dijkstra(graph, argv[2], argv[3], (double (*)(const void*)) DoubleContainer_get);
  print_path(graph, min_path);
  free(min_path);
  Graph_free(graph);
  return 0;
}
