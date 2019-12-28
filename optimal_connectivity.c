#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <math.h>
#include <time.h>

#define NUM_NODES 100001
#define LOG_NUM_NODES 17


typedef struct _Edge {
    int source;
    int dest;
    int weight;
} Edge;

typedef struct _Edges {
    Edge** edges;
    int num_edges;
} Edges;

typedef struct _Node {
    int elem;
    int weight;
    struct _Node *next;
} Node;

typedef struct _Tree {
    /// records the depth each node is found in the dfs visit
    int depth[NUM_NODES];

    /// ancestors[u][i] is the 2^i ancestor of node u, it can computed exploiting
    /// the following recurrence relation:
    ///    ancestors[u][0] = parent[u]
    ///    ancestors[u][i] = ancestors[ ancestors[u][i-1] ][i-1]
    /// where parent[u] is the parent of node u in a dfs visit of the graph.
    int ancestors[NUM_NODES][LOG_NUM_NODES];
    /// max_weights[u][i] is the max weight found in the path from u
    /// to its 2^i ancestor.
    /// It can be computed exploiting the following recurrence relation:
    ///    max_weights[u][0] = weight(u, parent[u])
    ///    max_weights[u][i] = max( max_weight[u][i-1], max_weight[ ancestors[u][i-1] ][i-1]
    /// expressing the idea that the max weight to the 2^i-th ancestor is either the max_weight
    /// to its 2^(i-1)-th ancestor or the max weight of that ancestor to its 2^(i-1)-th ancestor
    int max_weights[NUM_NODES][LOG_NUM_NODES];

    /// list of adjacency, adj[u] contains the list of adjacents of node u
    Node** adj;

    /// size of the tree
    int size;
} Tree;

// type of list iterators.
typedef Node ListIt;

#define max(a,b) (a) > (b) ? (a) : (b)

/// MARK: -------------------------   LIST   -------------------------

static int list_len(Node *list) {
    int count = 0;
    while(list != NULL) {
        list = list->next;
        count++;
    }

    return count;
}

static Node* list_add(Node* list, int elem, int weight) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->elem = elem;
    node->next = list;
    node->weight = weight;

    return node;
}

static void list_free(Node* list) {
    if(list == NULL) {
        return;
    }

    list_free(list->next);
    free(list);
}

// Returns a new list iterator. The list iterator needs not to be dealloced
static ListIt* list_iterator(Node* list) {
    return list;
}

// Moves the list iterator to the next element. Assign it to your current iterator. E.g.,
//
// ListIt* it = ...
// ...  
// it = list_iterator_next(it);
// 
// Returns NULL if no next element exists.
// Assumes that the iterator is valid (not NULL).

static ListIt* list_iterator_next(ListIt *it)
{
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
static int list_iterator_get_weight(ListIt *it)
{
    return it->weight;
}

/// MARK: -------------------------   TREE   -------------------------

// Initializes the tree. It assumes that the main structure has already been alloced.
static void tree_init(Tree *tree, int num_nodes)
{
    tree->adj = (Node **)calloc(num_nodes + 1, sizeof(Node *));
    tree->size = num_nodes;

    for(int u = 0; u<NUM_NODES; ++u) {
        tree->depth[u] = -1;
        for(int j=0; j<LOG_NUM_NODES; ++j) {
            tree->ancestors[u][j] = -1;
            tree->max_weights[u][j] = -1;
        }
    }    
}

// Adds the given edge to the tree.
// It assumes 0 <= e->source < NUM_NODES; 0 <= e->dest < NUM_NODES
static void tree_add_edge(Tree *tree, Edge *e)
{
    tree->adj[e->source] = list_add(tree->adj[e->source], e->dest, e->weight);
    tree->adj[e->dest] = list_add(tree->adj[e->dest], e->source, e->weight);
}

// Frees the memory occupied by the tree
static void tree_free(Tree* tree) {
    for(int u=0; u<tree->size; ++u) {
        list_free(tree->adj[u]);
    }

    free(tree->adj);
}

// Frees the memory occupied by the Edges data structure
static void edges_free(Edges* edges) {
    for(int i=0; i<edges->num_edges; ++i) {
        free(edges->edges[i]);
    }
    free(edges->edges);
}


/// MARK: -------------------------   I/O   -------------------------

// Reads the next integer from the stdin
static int read_num_edges() {
    int result;
    scanf("%d", &result);
    return result;
}

// Reads the next edge (three integers) from stdin
static void read_edge(Edge* edge) {
    scanf("%d %d %d", &edge->source, &edge->dest, &edge->weight);
}

// Reads a list of edges from stdin
static void read_edges(Tree* tree) {
    int num_edges = read_num_edges() - 1;
    tree_init(tree, num_edges+1);

    for(int i=0; i<num_edges; ++i) {
        Edge e;
        read_edge(&e);
        tree_add_edge(tree, &e);
    }
}

// Reads the query list from the stdin
static void read_queries(Edges* queries, Tree* tree) {
    queries->num_edges = read_num_edges();
    queries->edges = (Edge**) malloc(sizeof(Edge*)*queries->num_edges);

    for(int i=0; i<queries->num_edges; ++i) {
        queries->edges[i] = (Edge*) malloc(sizeof(Edge));
        read_edge(queries->edges[i]);
    }
}

/// MARK: -------------------------   ALGORITHM   -------------------------

/// Performs a depth first search on the given three starting from node u. In doing
/// it records:
///   - the depths at which each node is found;
///   - the list of 2^i-th ancestors for each node and for 0<=i<LOG_NUM_NODES
///   - the list of maximal weights for the ancestors

void depth_first_search(Tree* tree, int u) {
    // starts by updating ancestors and max weight for the current node, we assume
    // that the 0-th ancestor and the 0-th max weight have already been set and
    // also that all referenced ancestors already have their information set
    // (this is guaranteed by recursivity of the depth search visit)
    for(int i=1; i<LOG_NUM_NODES; ++i) {
        if(tree->ancestors[u][i-1]!=-1) {
            tree->ancestors[u][i] = tree->ancestors[ tree->ancestors[u][i-1] ][i-1];
            tree->max_weights[u][i] = max(tree->max_weights[ tree->ancestors[u][i-1] ][i-1], tree->max_weights[u][i-1]  );
        }
    }

    ListIt* it = list_iterator(tree->adj[u]);
    while(it!=NULL) {
        if(tree->depth[it->elem] != -1) {
            it = list_iterator_next(it);
            continue;
        }

        // sets the depth, the 0-th ancestor, and the 0-th max-value for the
        // next node;
        tree->depth[it->elem] = tree->depth[u] + 1;
        tree->ancestors[it->elem][0] = u;
        tree->max_weights[it->elem][0] = it->weight;
        depth_first_search(tree, it->elem);

        it = list_iterator_next(it);
    }
}

int ancestor(Tree* tree, int u, int level) {
    return tree->ancestors[u][level];
}

int maxval(Tree* tree, int u, int level) {
    return tree->max_weights[u][level];
}

// Returns the max weight found in the path from u=query->source to v=query->dest.
//
// The max weight is found by searching for the lowest common ancestor of
// the nodes u and v. If m_u is the max weight on the path from
// u to the lca(u,v) and m_v is the max weight on the path from v and lca(u,v),
// then max(m_u,m_v) is the max weight on the path from u to v.
int lca_max_weight(Tree* tree, Edge* query) {
    int u,v;
    int max_value = -1;
    if(tree->depth[query->source] > tree->depth[query->dest]) {
        u = query->source;
        v = query->dest;
    } else {
        u = query->dest;
        v = query->source;
    }

    // first we need to get both nodes at the same level. We do that
    // by raising the largest one until we get to the same level
    int depth_diff = tree->depth[u] - tree->depth[v];
    while(depth_diff > 0) {
        int log_diff = log2(depth_diff);
        
        max_value = max(max_value, maxval(tree, u, log_diff));
        u = ancestor(tree, u, log_diff);
        depth_diff = tree->depth[u] - tree->depth[v];
    }

    if(u==v) {
        return max_value;
    }

    // now we can raise both until we get to the lca
    int level = log2(tree->depth[u]);
    while (level >= 0)  {
        if (ancestor(tree, u, level) != -1 && ancestor(tree, u, level) != ancestor(tree, v, level)) {

            max_value = max(max_value, maxval(tree, u, level));
            max_value = max(max_value, maxval(tree, v, level));
            u = ancestor(tree, u, level);
            v = ancestor(tree, v, level);
        }

        level--;
    }


    return max(max_value, max(maxval(tree, v,0), maxval(tree, u,0)));
}


/// MARK: -------------------------   MAIN   -------------------------

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* filein = fopen(argv[1], "r");
        stdin = filein;
    }

    Tree* tree = (Tree*) malloc(sizeof(Tree));
    Edges* queries = (Edges*) malloc(sizeof(Edges));

    read_edges(tree);
    read_queries(queries, tree);

    int orig = queries->edges[0]->source;
    tree->depth[orig] = 0;
    depth_first_search(tree, queries->edges[0]->source);


    for(int i = 0; i < queries->num_edges; ++i) {
        if( lca_max_weight(tree, queries->edges[i]) > queries->edges[i]->weight ) {
            printf("YES\n");
        } else {
            printf("NO\n");        }
    }

    tree_free(tree);
    edges_free(queries);
    free(tree);
    free(queries);

    return 0;
}
