#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>

#include "dijkstra.h"
#include "dictionary.h"
#include "print_time.h"
#include "graph_visiting.h"
#include "double_container.h"
#include "errors.h"
#include "macros.h"
#include "kruskal.h"
#include "prim.h"
#include "array.h"
#include "iterator_functions.h"
#include "basic_iterators.h"
#include "mem.h"
#include "string_utils.h"
#include "ansi_colors.h"


#define BUF_SIZE 1024

static void add_edge(Graph* graph, Dictionary* known_vertices,  const char* v1, const char* v2, double len) {
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
    Graph_add_edge(graph, vertex1, vertex2, DoubleContainer_new(len));
    Graph_add_edge(graph, vertex2, vertex1, DoubleContainer_new(len));
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
    double len;

    sscanf((char*) obj, "%[^,],%[^,],%lf\n", buf1, buf2, &len);
    add_edge(graph, known_vertices, buf1, buf2, len);
    count += 2;
  });

  printf("dc creation count:" GRN "%d\n" reset, count);
  Dictionary_free(known_vertices);
  return graph;
}

static void destroy_graph_double_containers(Graph* graph) {
  __block int count = 0;

  for_each(Edge_it(graph), ^(void* obj) {
    count += 1;
    EdgeInfo* info = (EdgeInfo*) obj;
    DoubleContainer_free((DoubleContainer*)info->info);
  });

  printf("destroy dc count:" GRN "%d\n" reset, count);

  free_contents(Vertex_it(graph));
}

static void print_path(Graph* graph, void** path) {
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
      double len = DoubleContainer_get((DoubleContainer*)Graph_edge_info(graph, prev, current));
      printf("%20s %20s %8.2lfKm\n", (char*) prev, current, len/1000.0);
      path_len += len;
    }
  }

  printf("\nLen: %8.2lfKm\n", path_len/1000);
}

static void execute_dijkstra(Graph* graph, char* source, char* dest) {
  void** min_path;
  Dijkstra* d = Dijkstra_new(graph,(double (*)(const void*)) DoubleContainer_get);
  min_path = Dijkstra_minpath(d, source, dest);
  Dijkstra_free(d);

  print_path(graph, min_path);
  Mem_free(min_path);
}

static void check_edge(Graph* graph, const char* source, char* dest) {
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

  printf("%s -> %s (%8.2lfKm)", source, dest, DoubleContainer_get((DoubleContainer*)edge_info)/1000);
}

static void find_connected_components(Graph* graph, void (*graph_visit)(VisitingInfo*, void*, void(^)(void*))) {
  VisitingInfo* visit_info = VisitingInfo_new(graph);
  int count = 0;
  void* vertex;

  while( (vertex = VisitingInfo_next_unvisited(visit_info)) != NULL ) {
    __block size_t component_size = 0;
    graph_visit(visit_info, vertex, ^(void* UNUSED(v)) {
      component_size += 1;
    });
    count+=1;

    printf("Component %d size: %ld\n", count, component_size);
  }

  VisitingInfo_free(visit_info);
  printf("Number of connected components: %d\n", count);
}

static void execute_kruskal(Graph* graph) {
  Kruskal* kruskal = Kruskal_new(graph, (double (*)(const void*)) DoubleContainer_get);
  Graph* result = Kruskal_mintree(kruskal);

  __block double tree_size = 0.0;
  __block int num_edges = 0;
  __block double min_edge = 10000000;
  for_each(Edge_it(result), ^(void* obj) {
    EdgeInfo* ei = (EdgeInfo*) obj;
    double edge_weight = DoubleContainer_get(ei->info);
    tree_size += DoubleContainer_get(ei->info);
    num_edges += 1;

    if( edge_weight < min_edge ) {
      min_edge = edge_weight;
    }
  });

  printf("Original graph size: " GRN " %ld\n" reset, Graph_size(graph));
  printf("Result graph size: " GRN " %ld\n" reset, Graph_size(result));
  printf("Result graph num edges: " GRN " %d\n" reset, num_edges / 2);
  printf("Result graph min edge: " GRN " %f\n" reset, min_edge);

  // print_kruskal_edges(result);

  printf("Minimum spanning tree weight: " GRN "%f\n" reset, (tree_size / 2.0)/1000.0);
  Graph_free(result);
  Kruskal_free(kruskal);
}

static void execute_prim(Graph* graph) {
  void* start = first(Vertex_it(graph));
  Prim* prim = Prim_new(graph, (double (*)(const void*)) DoubleContainer_get);
  Graph* result = Prim_mintree(prim, start);

  __block double tree_size = 0.0;
  __block int num_edges = 0;
  __block double min_edge = 10000000;
  for_each(Edge_it(result), ^(void* obj) {
    EdgeInfo* ei = (EdgeInfo*) obj;
    double edge_weight = DoubleContainer_get(ei->info);
    tree_size += DoubleContainer_get(ei->info);
    num_edges += 1;

    if( edge_weight < min_edge ) {
      min_edge = edge_weight;
    }
  });

  printf("Original graph size: " GRN "%ld\n" reset, Graph_size(graph));
  printf("Result graph size: " GRN "%ld\n" reset, Graph_size(result));
  printf("Result graph num edges: " GRN "%d\n" reset, num_edges / 2);
  printf("Result graph min edge: " GRN "%f\n" reset, min_edge);

  printf("Minimum spanning tree weight: " GRN "%f\n" reset, (tree_size / 2.0)/1000.0);
  Graph_free(result);
  Prim_free(prim);
}

static void execute_print_graphviz(Graph* graph) {
  FILE* file = fopen("graph.dot", "w");
  fprintf(file, "digraph G {\n");
  for_each(Edge_it(graph), ^(void* obj) {
    EdgeInfo* ei = (EdgeInfo*) obj;

    fprintf(file, "\"%s\" ->  \"%s\" [label = \"%f\"];\n", ei->source, ei->destination, DoubleContainer_get(ei->info) );

  });
  fprintf(file, "}\n");
  fclose(file);
}

static void print_usage(const char* msg) {
  printf("%s\n\n",msg);
  printf("Usage: measure_times <op specifier> <graph file name> <source> <dest>\n");
  printf("where <op specifier> can be:\n");
  printf("  -d: to invoke dijkstra\n");
  printf("  -e: to check edge existence\n");
  printf("  -c: to find connected components using dfs\n");
  printf("  -b: to find connected components using bfs\n");
  printf("  -k: to invoke kruskal\n");
  printf("  -p: to invoke prim\n");
  printf("  -P: to print the graph in graphviz format");
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
  if(op!='d' && op!='e' && op!='c' && op!='b' && op!='k' && op!='p' && op!='P') {
    print_usage("Op specifier needs to be one of {-d,-e,-c,-b,-k,-p,-P}");
    exit(ERROR_ARGUMENT_PARSING);
  }

  switch(op) {
    case 'c':
    case 'b':
    case 'k':
    case 'p':
    case 'P':
      if(argc != 3) {
        print_usage("Wrong number of arguments - expected 3");
        exit(ERROR_ARGUMENT_PARSING);
      }
      break;
    case 'd':
    case 'e':
      if(argc != 5) {
        print_usage("Wrong number of arguments - expected 5");
        exit(ERROR_ARGUMENT_PARSING);
      }
      break;
    default:
      break;
  }
}

static char* flag_to_task_name(char ch) {
  switch(ch) {
    case 'd': return "dijkstra";
    case 'c': return "connected_components_dfs";
    case 'b': return "connected_components_bfs";
    case 'e': return "edge_check";
    case 'k': return "kruskal";
    case 'p': return "prim";
    case 'P': return "print graph in graphviz format";
    default:
      printf("Unknown flag");
      exit(ERROR_ARGUMENT_PARSING);
  }
}

static char* get_compilation_flags() {
  static char buf[4096];
  Array* strings = Array_new(5);

  for_each(TextFile_it("Makefile.vars",'\n'), ^(void* obj){
    char* line = (char*) obj;
    if(line[0] == '\0' || line[0] == '#') {
      return;
    }

    if(strstr(line, "CFLAGS")!=NULL) {
      Array_add(strings, Mem_strdup(line));
    }
  });

  char* result = String_join(strings, ' ');
  strcpy(buf, result);

  Mem_free(result);

  free_contents(Array_it(strings));

  Array_free(strings);

  return buf;
}

static PrintTime* init_print_time(char* argv[]) {
  PrintTime* pt = PrintTime_new(NULL);

  PrintTime_add_header(pt,"esercizio", "3");
  PrintTime_add_header(pt, "task", flag_to_task_name(argv[1][1]));
  PrintTime_add_header(pt, "invocation", argv[0]);
  PrintTime_add_header(pt, "compilation_flags", get_compilation_flags());

  return pt;
}

int main(int argc, char *argv[]) {
  check_arguments(argc, argv);
  PrintTime* pt = init_print_time(argv);


  __block Graph* graph;
  PrintTime_print(pt, "Graph_load", ^{
    printf("Loading graph...\n");
    graph = load_graph(argv[2]);
    printf("Graph size: " GRN "%ld\n" reset, Graph_size(graph));
  });

  PrintTime_print(pt, "Algorithm_execution", ^{
    switch(argv[1][1]) {
      case 'd':
        printf("Executing Dijkstra* algorithm...\n");
        execute_dijkstra(graph, argv[3], argv[4]);
        break;
      case 'e':
        printf("Checking edge presence...\n");
        check_edge(graph, argv[3], argv[4]);
        break;
      case 'c':
        printf("Finding connected components (dfs)...\n");
        find_connected_components(graph, Graph_depth_first_visit);
        break;
      case 'b':
        printf("Finding connected components (bfs)...\n");
        find_connected_components(graph, Graph_breadth_first_visit);
        break;
      case 'k':
        printf("Executing kruskal...\n");
        execute_kruskal(graph);
        break;
      case 'P':
        printf("Printing graph in GraphViz format...\n");
        execute_print_graphviz(graph);
        break;
      case 'p':
        printf("Executing prim...\n");
        execute_prim(graph);
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

  Mem_check_and_report();
  return 0;
}
