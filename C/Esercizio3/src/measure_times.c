#include "dijkstra.h"
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include "print_time.h"
#include "graph_visiting.h"
#include <assert.h>

#define BUF_SIZE 1024

void add_edge(Graph graph, Dictionary known_vertices,  const char* v1, const char* v2, double len) {
  // printf("inserting %s -> %s (%lf)\n", v1, v2, len);

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

  if(!Graph_has_edge(graph, vertex1, vertex2)) {
    Graph_add_edge(graph, vertex1, vertex2, DoubleContainer_new(len));
    Graph_add_edge(graph, vertex2, vertex1, DoubleContainer_new(len));
  }
}

Graph load_graph(const char* filename) {
  KeyInfo keyInfo = KeyInfo_new( Dictionary_string_compare, Dictionary_string_hash );
  Dictionary known_vertices = Dictionary_new(keyInfo);
  Graph graph = Graph_new(keyInfo);
  FILE* infile = fopen(filename, "r");
  int count = 0;

  while(!feof(infile)) {
    char buf1[BUF_SIZE];
    char buf2[BUF_SIZE];
    double len;

    fscanf(infile, "%[^,],%[^,],%lf\n", buf1, buf2, &len);
    add_edge(graph, known_vertices, buf1, buf2, len);
    count += 2;
  }

  printf("dc creation count:%d\n", count);

  fclose(infile);

  Dictionary_free(known_vertices);
  return graph;
}

void destroy_graph_double_containers(Graph graph) {
  int count = 0;
  EdgeIterator it = Graph_edges(graph);
  while(!EdgeIterator_end(it)) {
    count += 1;
    EdgeInfo info = EdgeIterator_get(it);
    DoubleContainer_free((DoubleContainer)info.info);
    EdgeIterator_next(it);
  }
  EdgeIterator_free(it);

  printf("destroy dc count:%d\n", count);

  VertexIterator v_it = Graph_vertices(graph);
  while(!VertexIterator_end(v_it)) {
    free((void*)VertexIterator_get(v_it));
    VertexIterator_next(v_it);
  }
  VertexIterator_free(v_it);
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

void execute_dijkstra(Graph graph, char const* source, char const* dest) {
  const void** min_path;
  Dijkstra d = Dijkstra_new(graph,(double (*)(const void*)) DoubleContainer_get);
  min_path = Dijkstra_minpath(d, source, dest);
  Dijkstra_free(d);

  print_path(graph, min_path);
  free(min_path);
}

void check_edge(Graph graph, const char* source, const char* dest) {
  if(!Graph_has_vertex(graph, source)) {
    printf("vertex %s not in the graph\n", source);
    return;
  }

  if(!Graph_has_vertex(graph, dest)) {
    printf("vertex %s not in the graph\n", dest);
    return;
  }

  if(!Graph_has_edge(graph, source, dest)) {
    printf("edge %s -> %s not in the graph\n", source, dest);
    return;
  }

  const void* edge_info = Graph_edge_info(graph, source, dest);

  printf("%s -> %s (%8.2lfKm)", source, dest, DoubleContainer_get((DoubleContainer)edge_info)/1000);
}

void visit_vertex(const void* vertex, void* user_info) {
  Dictionary dic = (Dictionary) user_info;
  Dictionary_set(dic, vertex, "VISITED");
}

const void* unvisited_vertex(Graph graph, Dictionary visited_vertices) {
  const void* result = NULL;
  const void* dummy;
  VertexIterator v_it = Graph_vertices(graph);
  while(!VertexIterator_end(v_it) && result==NULL) {
    if(!Dictionary_get(visited_vertices, VertexIterator_get(v_it), &dummy)) {
      result = VertexIterator_get(v_it);
    }

    VertexIterator_next(v_it);
  }
  VertexIterator_free(v_it);

  return result;
}

void find_connected_components(Graph graph) {
  Dictionary visited_vertices = Dictionary_new(Graph_keyInfo(graph));
  VertexIterator v_it;
  VisitingInfo visit_info = VisitingInfo_new(graph, visit_vertex, visited_vertices);
  int stop = 0;
  int count = 0;
  const void* vertex;

  while( (vertex = unvisited_vertex(graph, visited_vertices)) != NULL ) {
    Graph_depth_first_visit(visit_info, vertex);
    count+=1;
  }

  VisitingInfo_free(visit_info);
  printf("Number of connected components: %d\n", count);
}

void print_usage(const char* msg) {
  printf("%s\n\n",msg);
  printf("Usage: measure_times <op specifier> <graph file name> <source> <dest>\n");
  printf("where <op specifier> can be:\n");
  printf("  -d: to invoke dijkstra\n");
  printf("  -e: to check edge existence\n");
  printf("  -c: to find connected components");
}

void check_arguments(int argc, char const* argv[]) {
  if(argc < 3) {
    print_usage("Wrong number of arguments.");
    exit(1);
  }

  if(argv[1][0]!='-') {
    print_usage("First argument needs to be the op specifier");
    exit(1);
  }
  char op = argv[1][1];
  if(op!='d' && op!='e' && op!='c') {
    print_usage("Op specifier needs to be one of {-d,-e,-c}");
    exit(1);
  }

  if(op!='c' && argc != 5)  {
    print_usage("Wrong number of arguments");
    exit(1);
  }

  if(op=='c' && argc != 3) {
    print_usage("Wrong number of arguments");
    exit(1);
  }
}


int main(int argc, char const *argv[]) {
  check_arguments(argc, argv);

  __block Graph graph;
  print_time(^{
    printf("Loading graph...\n");
    graph = load_graph(argv[2]);
  });

  print_time(^{
    switch(argv[1][1]) {
      case 'd':
        printf("Executing Dijkstra algorithm...\n");
        execute_dijkstra(graph, argv[3], argv[4]);
        break;
      case 'e':
        printf("Checking edge presence...\n");
        check_edge(graph, argv[3], argv[4]);
        break;
      case 'c':
        printf("Finding connected components...\n");
        find_connected_components(graph);
        break;
    }
  });

  printf("Freeing memory\n");
  destroy_graph_double_containers(graph);
  KeyInfo_free(Graph_keyInfo(graph));
  Graph_free(graph);
  return 0;
}
