#include "dijkstra.h"
#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include "print_time.h"
#include "graph_visiting.h"
#include <assert.h>
#include "double_container.h"
#include "errors.h"
#include "macros.h"
#include <errno.h>

#define BUF_SIZE 1024

static void add_edge(Graph graph, Dictionary known_vertices,  const char* v1, const char* v2, double len) {
  // printf("inserting %s -> %s (%lf)\n", v1, v2, len);

  void* vertex1 = NULL;
  void* vertex2 = NULL;

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

static Graph load_graph(const char* filename) {
  KeyInfo keyInfo = KeyInfo_new( Key_string_compare, Key_string_hash );
  Dictionary known_vertices = Dictionary_new(keyInfo);
  Graph graph = Graph_new(keyInfo);
  FILE* infile = fopen(filename, "r");
  if(infile==NULL) {
    Error_raise( Error_new( ERROR_FILE_OPENING, "Cannot open file %s for reading, reason: %s", filename, strerror(errno)));
  }


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

static void destroy_graph_double_containers(Graph graph) {
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

static void print_path(Graph graph, void** path) {
  if(path==NULL) {
    printf("Empty path!\n");
    return;
  }

  double path_len = 0.0;
  int index = 0;
  void* current = path[index++];
  void* prev = NULL;
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

static void execute_dijkstra(Graph graph, char* source, char* dest) {
  void** min_path;
  Dijkstra d = Dijkstra_new(graph,(double (*)(const void*)) DoubleContainer_get);
  min_path = Dijkstra_minpath(d, source, dest);
  Dijkstra_free(d);

  print_path(graph, min_path);
  free(min_path);
}

static void check_edge(Graph graph, const char* source, char* dest) {
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

  void* edge_info = Graph_edge_info(graph, source, dest);

  printf("%s -> %s (%8.2lfKm)", source, dest, DoubleContainer_get((DoubleContainer)edge_info)/1000);
}

static void find_connected_components(Graph graph) {
  VisitingInfo visit_info = VisitingInfo_new(graph);
  int count = 0;
  void* vertex;

  while( (vertex = VisitingInfo_next_unvisited(visit_info)) != NULL ) {
    __block size_t component_size = 0;
    Graph_depth_first_visit(visit_info, vertex, ^(void* UNUSED(v)) {
      component_size += 1;
    });
    count+=1;

    printf("Component %d size: %ld\n", count, component_size);
  }

  VisitingInfo_free(visit_info);
  printf("Number of connected components: %d\n", count);
}

static void print_usage(const char* msg) {
  printf("%s\n\n",msg);
  printf("Usage: measure_times <op specifier> <graph file name> <source> <dest>\n");
  printf("where <op specifier> can be:\n");
  printf("  -d: to invoke dijkstra\n");
  printf("  -e: to check edge existence\n");
  printf("  -c: to find connected components");
}

static void check_arguments(int argc, char* argv[]) {
  if(argc < 3) {
    print_usage("Wrong number of arguments.");
    exit(ERROR_ARGUMENT_PARSING);
  }

  if(argv[1][0]!='-') {
    print_usage("First argument needs to be the op specifier");
    exit(ERROR_ARGUMENT_PARSING);
  }
  char op = argv[1][1];
  if(op!='d' && op!='e' && op!='c') {
    print_usage("Op specifier needs to be one of {-d,-e,-c}");
    exit(ERROR_ARGUMENT_PARSING);
  }

  if(op!='c' && argc != 5)  {
    print_usage("Wrong number of arguments");
    exit(ERROR_ARGUMENT_PARSING);
  }

  if(op=='c' && argc != 3) {
    print_usage("Wrong number of arguments");
    exit(ERROR_ARGUMENT_PARSING);
  }
}

static char* flag_to_task_name(char ch) {
  switch(ch) {
    case 'd': return "dijkstra";
    case 'c': return "connected_components";
    case 'e': return "edge_check";
    default:
      printf("Unknown flag");
      exit(ERROR_ARGUMENT_PARSING);
  }
}

static char* get_compilation_flags() {
  static char buf[4096];
  FILE* file = fopen("Makefile.vars", "r");
  if(file==NULL) {
    Error_raise(Error_new(ERROR_FILE_READING, "Cannot open Makefile.vars to read compilatin flags"));
  }

  int found = 0;
  while( !found && fgets(buf, 4096, file) != NULL) {
    found =  strstr(buf, "CFLAGS") != NULL;
  }

  if(!found) {
    Error_raise(Error_new(ERROR_FILE_READING, "Cannot find CFLAGS string into Makefile.vars"));
  }

  buf[strlen(buf)-1] = '\0'; // removing trailing \n
  return buf;
}

static PrintTime init_print_time(char* argv[]) {
  PrintTime pt = PrintTime_new(NULL);

  PrintTime_add_header(pt,"esercizio", "3");
  PrintTime_add_header(pt, "task", flag_to_task_name(argv[1][1]));
  PrintTime_add_header(pt, "invocation", argv[0]);
  PrintTime_add_header(pt, "compilation_flags", get_compilation_flags());

  return pt;
}

int main(int argc, char *argv[]) {
  check_arguments(argc, argv);
  PrintTime pt = init_print_time(argv);


  __block Graph graph;
  PrintTime_print(pt, "Graph_load", ^{
    printf("Loading graph...\n");
    graph = load_graph(argv[2]);
    printf("Graph size: %ld\n", Graph_size(graph));
  });

  PrintTime_print(pt, "Algorithm_execution", ^{
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

  PrintTime_print(pt, "Graph_free", ^{
    printf("Freeing memory\n");
    destroy_graph_double_containers(graph);
    KeyInfo_free(Graph_keyInfo(graph));
    Graph_free(graph);
  });

  PrintTime_save(pt);
  PrintTime_free(pt);
  return 0;
}
