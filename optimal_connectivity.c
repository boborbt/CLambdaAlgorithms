#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <math.h>
#include <time.h>

#define NUM_NODES 100001
#define LOG_NUM_NODES 17

typedef enum {
    UNKNOWN=0,
    YES,
    NO
} Answer;

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
    /// ancestors[u][i] is the 2^i ancestor of node u
    int ancestors[NUM_NODES][LOG_NUM_NODES];
    /// max_weights[u][i] is the max weight found in the pat from u
    /// to its 2^i ancestor
    int max_weights[NUM_NODES][LOG_NUM_NODES];

    /// list of adjacency, adj[u] contains the list of adjacents of node u
    Node** adj;

    /// size of the tree
    int size;
} Tree;


typedef Node ListIt;

#define max(a,b) (a) > (b) ? (a) : (b)
static Edge *bin_search(Edges *edges, Edge *goal);
static int compare_edges(Edge ** e1, Edge** e2);

/// MARK: -------------------------   DEBUG   -------------------------

static void print_edge(Edge e)
{
    printf("s:%d d:%d w:%d", e.source, e.dest, e.weight);
}

static void print_edges(Edges *edges, int start, int end)
{
    for (int i = start; i <= end; ++i)
    {
        print_edge(*edges->edges[i]);
        printf("\n");
    }
}

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

static Node* list_remove(Node* list, int elem) {
    assert(list != NULL);
    if(list->elem == elem) {
        Node* next = list->next;
        free(list);

        return next;
    }

    list->next = list_remove(list->next, elem);
    return list;
}

static ListIt* list_iterator(Node* list) {
    return list;
}

static ListIt* list_iterator_next(ListIt *list)
{
    return list->next;
}

static int list_iterator_get(ListIt* it) {
    return it->elem;
}

static int list_iterator_get_weight(ListIt *it)
{
    return it->weight;
}

/// MARK: -------------------------   TREE   -------------------------

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

static void tree_add_edge(Tree *tree, Edge *e)
{
    tree->adj[e->source] = list_add(tree->adj[e->source], e->dest, e->weight);
    tree->adj[e->dest] = list_add(tree->adj[e->dest], e->source, e->weight);
}



/// MARK: -------------------------   I/O   -------------------------

static int read_num_edges() {
    int result;
    scanf("%d", &result);
    return result;
}


static void read_edge(Edge* edge) {
    scanf("%d %d %d", &edge->source, &edge->dest, &edge->weight);
}

static void read_edges(Tree* tree) {
    int num_edges = read_num_edges() - 1;
    tree_init(tree, num_edges+1);

    for(int i=0; i<num_edges; ++i) {
        Edge e;
        read_edge(&e);
        tree_add_edge(tree, &e);
    }
}


static void read_queries(Edges* queries, Tree* tree) {
    queries->num_edges = read_num_edges();
    queries->edges = (Edge**) malloc(sizeof(Edge*)*queries->num_edges);

    for(int i=0; i<queries->num_edges; ++i) {
        queries->edges[i] = (Edge*) malloc(sizeof(Edge));
        read_edge(queries->edges[i]);
    }
}

static void print_answer(Answer answer) {
    switch(answer) {
        case UNKNOWN:
            printf("ERRORE\n");
            exit(1);
        case YES:
            printf("YES\n");
            break;
        case NO:
            printf("NO\n");
            break;
    }
}

/// MARK: -------------------------   ALGORITHM   -------------------------

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

// The max weight is found by searching for the lowest common ancestor of
// the nodes u and v in the query. If m_u is the max weight on the path from
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
        max_value = max( max_value, tree->max_weights[u][log_diff] );
        u = tree->ancestors[u][log_diff];
        depth_diff = tree->depth[u] - tree->depth[v];
    }

    if(u==v) {
        return max_value;
    }

    // now we can raise both until we get to the lca
    int level = log2(tree->depth[u]);
    while(tree->ancestors[u][level]!=tree->ancestors[v][level]) {

        max_value = max( max_value, tree->max_weights[u][level]);
        max_value = max( max_value, tree->max_weights[v][level]);
        u = tree->ancestors[u][level];
        v = tree->ancestors[v][level];
        level--;
    }

    return max( max(max_value, tree->max_weights[u][0]), tree->max_weights[v][0]);
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

    tree->depth[queries->edges[0]->source] = 0;
    depth_first_search(tree, queries->edges[0]->source);

    for(int i = 0; i < queries->num_edges; ++i) {
        if( lca_max_weight(tree, queries->edges[i]) > queries->edges[i]->weight ) {
            printf("YES\n");
        } else {
            printf("NO\n");        }
    }

    return 0;
}
