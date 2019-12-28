#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <math.h>
#include <time.h>

#define NUM_NODES 100001
#define LOG_NUM_NODES 20


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

int ancestor(Tree* tree, int u, int level) {
    return tree->ancestors[u][level];
}

int maxval(Tree* tree, int u, int level) {
    return tree->max_weights[u][level];
}

void print_table_line(int u, int* line) {
    printf("printing table line for node: %d\n", u);
    for(int i=0; i<LOG_NUM_NODES; ++i) {
        printf("%d:%d ", i, line[i]);
    }
    printf("\n\n");
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

        #ifdef DBG_QRY
            printf("u:%d v:%d depth[u]:%d depth[v]: %d depth diff:%d log diff:%d\n", u, v, tree->depth[u], tree->depth[v], depth_diff, log_diff);
        #endif
        
        max_value = max(max_value, maxval(tree, u, log_diff));
        u = ancestor(tree, u, log_diff);
        depth_diff = tree->depth[u] - tree->depth[v];
    }

    #ifdef DBG_QRY
        printf("u:%d v:%d depth[u]:%d depth[v]: %d depth diff:%d\n", u, v, tree->depth[u], tree->depth[v], depth_diff);
    #endif

    if(u==v) {
        return max_value;
    }

    #ifdef DBG_QRY
        print_table_line(u, tree->ancestors[u]);
        print_table_line(v, tree->ancestors[v]);
    #endif

    #ifdef DBG_QRY
        printf("initial same level ancestors  u:%d and v:%d\n", ancestor(tree, u, 0), ancestor(tree, v, 0));
    #endif

    // now we can raise both until we get to the lca
    int level = LOG_NUM_NODES - 1;
    while (level >= 0)
    {
        if (ancestor(tree, u, level) != -1 && ancestor(tree, u, level) != ancestor(tree, v, level))
        {
            #ifdef DBG_QRY
                printf("updating u,v for level: %d; a(u):%d a(v):%d\n", level, ancestor(tree,u,level), ancestor(tree, v, level));
            #endif
            max_value = max(max_value, maxval(tree, u, level));
            max_value = max(max_value, maxval(tree, v, level));
            u = ancestor(tree, u, level);
            v = ancestor(tree, v, level);
        }

        level--;
    }

    #ifdef DBG_QRY
        printf("final ancestors  u:%d and v:%d\n", ancestor(tree, u, 0), ancestor(tree, v, 0));
    #endif


    return max(max_value, max(maxval(tree, v,0), maxval(tree, u,0)));
}


int find_path_(Tree* tree, int source, int end, int* visited, Node* path[NUM_NODES],int level) {
    if(source == end) {
        return level;
    }

    visited[source] = 1;
    ListIt* it = list_iterator(tree->adj[source]);
    while(it!=NULL) {
        if(visited[it->elem]) {
            it = list_iterator_next(it);
            continue;
        }

        int found = find_path_(tree, it->elem, end, visited, path, level + 1);
        if(found) {
            path[level] = it;
            return found;
        }

        it = list_iterator_next(it);
    }

    return 0;
}

void print_path(Tree* tree, int source, int end) {
    int* visited = (int*) malloc(sizeof(int[NUM_NODES]));
    Node** path = (Node**) malloc(sizeof(Node*[NUM_NODES]));

    for(int i=0; i<NUM_NODES; ++i) {
        visited[i] = 0;
    }

    int max_index = find_path_(tree, source, end, visited, path, 0);
    printf("0:\t\t%d\t--(%d)-- %d\n", source, path[0]->weight, path[0]->elem);
    for(int i=1; i<max_index; ++i) {
        printf("%d:\t\t\t--(%d)-- %d\n", i, path[i]->weight, path[i]->elem);
    }

    printf("\n\n");
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

    #ifdef DBG_QRY
        int dbg_query = DBG_QRY - 1;
        printf("Debugging query number: %d (s:%d d:%d w:%d) width orig:%d\n\n", DBG_QRY, queries->edges[dbg_query]->source, queries->edges[dbg_query]->dest, queries->edges[dbg_query]->weight, orig);
        printf("lca_w:%d w:%d\n", lca_max_weight(tree, queries->edges[dbg_query]), queries->edges[dbg_query]->weight);

        printf("printing path between %d -- %d\n", queries->edges[dbg_query]->source, queries->edges[dbg_query]->dest );
        print_path(tree, queries->edges[dbg_query]->source, queries->edges[dbg_query]->dest);

        printf("printing path between orig(%d) and %d\n", orig, queries->edges[dbg_query]->source);
        print_path(tree, orig, queries->edges[dbg_query]->source);

        printf("printing path between orig(%d) and %d\n", orig, queries->edges[dbg_query]->dest);
        print_path(tree, orig, queries->edges[dbg_query]->dest);
    #else
        for(int i = 0; i < queries->num_edges; ++i) {
            if( lca_max_weight(tree, queries->edges[i]) > queries->edges[i]->weight ) {
                printf("YES\n");
            } else {
                printf("NO\n");        }
        }
    #endif

    return 0;
}
