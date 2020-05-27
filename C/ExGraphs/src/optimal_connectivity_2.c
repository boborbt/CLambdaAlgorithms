#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#define NUM_NODES 100005

static void swap(int* v1, int* v2) {
  int tmp = *v1;
  *v1 = *v2;
  *v2 = tmp;
}

#define require(a)                                             \
  if (!(a)) {                                                  \
    fprintf(stderr, "Require failed at line:%d!\n", __LINE__); \
    exit(1);                                                   \
  }

typedef struct _Edge {
  int source;
  int dest;
  int weight;
} Edge;


// we know the compiler will add padding to _Edges structure
// to improve memory access times. There is nothing we can do
// to avoid it so we are silencing the warning.

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wpadded"

typedef struct _Edges {
  int num_edges;
  Edge** edges;
} Edges;

#pragma clang diagnostic pop

typedef struct _Node {
  int elem;
  int weight;
  struct _Node* next;
} Node;

typedef struct _Tree {
  /// records the depth each node is found in the dfs visit
  int ancestors[NUM_NODES];
  int weights[NUM_NODES];
  int levels[NUM_NODES];

  /// size of the tree
  int size;

  /// list of adjacency, adj[u] contains the list of adjacents of node u
  Node** adj;

} Tree;

// type of list iterators.
typedef Node ListIt;

#define max(a, b) (a) > (b) ? (a) : (b)

/// MARK: -------------------------   LIST   -------------------------

// ..Unused..
// static int list_len(Node* list) {
//   int count = 0;
//   while (list != NULL) {
//     list = list->next;
//     count++;
//   }

//   return count;
// }

static Node* list_add(Node* list, int elem, int weight) {
  Node* node = (Node*)malloc(sizeof(Node));
  node->elem = elem;
  node->next = list;
  node->weight = weight;

  return node;
}

static void list_free(Node* list) {
  if (list == NULL) {
    return;
  }

  list_free(list->next);
  free(list);
}

// Returns a new list iterator. The list iterator needs not to be dealloced
static ListIt* list_iterator(Node* list) { return list; }

// Moves the list iterator to the next element. Assign it to your current
// iterator. E.g.,
//
// ListIt* it = ...
// ...
// it = list_iterator_next(it);
//
// Returns NULL if no next element exists.
// Assumes that the iterator is valid (not NULL).

static ListIt* list_iterator_next(ListIt* it) {
  assert(it != NULL);
  return it->next;
}

// Returns the element currently pointed by this iterator.
// Assumes that the iterator is valid.
static int list_iterator_get(ListIt* it) {
  assert(it != NULL);
  return it->elem;
}

// Returns the weight currently pointed by this iterator.
// Assumes that the iterator is valid.
static int list_iterator_get_weight(ListIt* it) { return it->weight; }

/// MARK: -------------------------   TREE   -------------------------

// Initializes the tree. It assumes that the main structure has already been
// alloced.
static void tree_init(Tree* tree, int num_nodes) {
  tree->adj = (Node**)calloc((unsigned long)num_nodes + 1, sizeof(Node*));
  tree->size = num_nodes;

  for(int i =0; i<NUM_NODES; ++i) {
    tree->ancestors[i] = -2;
    tree->weights[i] = -1;
    tree->levels[i] = -1;
  }
}

// Adds the given edge to the tree.
// It assumes 0 <= e->source < NUM_NODES; 0 <= e->dest < NUM_NODES
static void tree_add_edge(Tree* tree, Edge* e) {
  tree->adj[e->source] = list_add(tree->adj[e->source], e->dest, e->weight);
  tree->adj[e->dest] = list_add(tree->adj[e->dest], e->source, e->weight);
}

// Frees the memory occupied by the tree
static void tree_free(Tree* tree) {
  for (int u = 0; u < tree->size; ++u) {
    list_free(tree->adj[u]);
  }

  free(tree->adj);
}

// Frees the memory occupied by the Edges data structure
static void edges_free(Edges* edges) {
  for (int i = 0; i < edges->num_edges; ++i) {
    free(edges->edges[i]);
  }
  free(edges->edges);
}

/// MARK: -------------------------   I/O   -------------------------

// Reads the next integer from the stdin
static int read_num_edges() {
  int result;
  require(scanf("%d", &result) == 1);
  return result;
}

// Reads the next edge (three integers) from stdin
static void read_edge(Edge* edge) {
  require(scanf("%d %d %d", &edge->source, &edge->dest, &edge->weight) == 3);
}

// Reads a list of edges from stdin
static void read_edges(Tree* tree) {
  int num_edges = read_num_edges() - 1;
  require(num_edges < NUM_NODES - 1);

  tree_init(tree, num_edges + 1);

  for (int i = 0; i < num_edges; ++i) {
    Edge e;
    read_edge(&e);
    tree_add_edge(tree, &e);
  }
}

// Reads the query list from the stdin
static void read_queries(Edges* queries) {
  queries->num_edges = read_num_edges();
  require(queries->num_edges < NUM_NODES);

  queries->edges = (Edge**)malloc(sizeof(Edge*) * (unsigned long) queries->num_edges);
  require(queries->edges != NULL);

  for (int i = 0; i < queries->num_edges; ++i) {
    queries->edges[i] = (Edge*)malloc(sizeof(Edge));
    require(queries->edges[i] != NULL);

    read_edge(queries->edges[i]);
  }
}

/// MARK: -------------------------   ALGORITHM   -------------------------


/// Performs a depth first search on the given three starting from node u. In
/// doing it records:
///   - the depths at which each node is found;
///   - the parents for each node during the visit
///   - the weight of the edge from a node and its parent in the visit

static void depth_first_search(Tree* tree, int u) {
  require(u <= tree->size);

  ListIt* it = list_iterator(tree->adj[u]);
  while (it != NULL) {
    int elem = list_iterator_get(it);

    if(tree->ancestors[elem]!=-2) {
      it = list_iterator_next(it);
      continue;
    }

    int weight = list_iterator_get_weight(it);
    tree->ancestors[elem] = u;
    tree->weights[elem] = weight;
    tree->levels[elem] = tree->levels[u] + 1;
    depth_first_search(tree, elem);

    it = list_iterator_next(it);
  }
}


// Returns the max weight found in the path from u=query->source to
// v=query->dest.
//
// The max weight is found by searching for the lowest common ancestor of
// the nodes u and v. If m_u is the max weight on the path from
// u to lca(u,v) and m_v is the max weight on the path from v to lca(u,v),
// then max(m_u,m_v) is the max weight on the path from u to v.
static int lca_max_weight(Tree* tree, Edge* query) {
  require(query->source <= tree->size);
  require(query->dest <= tree->size);

  int u, v;
  int max_value = -1;

  u = query->source;
  v = query->dest;

  if(tree->levels[u] < tree->levels[v]) {
    swap(&u, &v);
  }

  while(tree->levels[u] > tree->levels[v]) {
    int p = tree->ancestors[u];
    max_value = max(max_value, tree->weights[u]);
    u = p;
  }

  // now we can raise both until we get to the lca
  while (u!=v) {
    int pu = tree->ancestors[u];
    int pv = tree->ancestors[v];

    max_value = max(max_value, tree->weights[u]);
    max_value = max(max_value, tree->weights[v]);

    u = pu;
    v = pv;
  }

  return max_value;
}

/// MARK: -------------------------   MAIN   -------------------------

static void main_work() {
  Tree* tree = (Tree*)malloc(sizeof(Tree));
  Edges* queries = (Edges*)malloc(sizeof(Edges));
  require(tree != NULL);
  require(queries != NULL);

  read_edges(tree);
  read_queries(queries);

  int orig = queries->edges[0]->source;
  tree->ancestors[orig] = -1;
  tree->levels[orig] = 0;
  depth_first_search(tree, orig);

  for (int i = 0; i < queries->num_edges; ++i) {
    if (lca_max_weight(tree, queries->edges[i]) > queries->edges[i]->weight) {
      printf("YES\n");
    } else {
      printf("NO\n");
    }
  }

  tree_free(tree);
  edges_free(queries);
  free(tree);
  free(queries);
}

static void open_files(int argc, char** argv) {
  if (argc > 1) {
    FILE* filein = fopen(argv[1], "r");
    stdin = filein;
  }

  if (argc > 2) {
    FILE* fileout = fopen(argv[2], "w");
    stdout = fileout;
  }
}

static void close_files(int argc) {
  if (argc > 1) {
    fclose(stdin);
  }

  if (argc > 2) {
    fclose(stdout);
  }
}

/// MARK: -------------------------   MAIN ENTRY POINT -------------------------

int main(int argc, char** argv) {
  open_files(argc, argv);

  main_work();

  close_files(argc);
  return 0;
}
