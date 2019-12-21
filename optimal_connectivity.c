#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>

#define NUM_NODES 100000

typedef struct _Edge {
    int source;
    int dest;
    int weight;
} Edge;

typedef struct _Edges {
    Edge edges[NUM_NODES];
    int num_edges;
} Edges;

typedef struct _Node {
    int elem;
    struct _Node *next;
} Node;

typedef struct _Tree {
    int visited[NUM_NODES];
    Node** adj;
    int size;
    int max_w;
    int cur_mark;
} Tree;


typedef Node ListIt;

#define max(a,b) (a) > (b) ? (a) : (b)

/// MARK: -------------------------   LIST   -------------------------

static int list_len(Node* list) {
    int count = 0;
    while(list != NULL) {
        list = list->next;
        count++;
    }

    return count;
}

static Node* list_add(Node* list, int elem) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->elem = elem;
    node->next = list;

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

/// MARK: -------------------------   TREE   -------------------------

static void tree_init(Tree *tree, int num_nodes)
{
    tree->adj = (Node **)calloc(num_nodes + 1, sizeof(Node *));
    tree->size = num_nodes;
    bzero(tree->visited, tree->size * sizeof(int));
    tree->cur_mark = 1;
}

static void tree_add_edge(Tree *tree, Edge *e)
{
    tree->adj[e->source] = list_add(tree->adj[e->source], e->dest);
    tree->adj[e->dest] = list_add(tree->adj[e->dest], e->source);
    tree->max_w = max(tree->max_w, e->weight);
}

static void tree_build(Tree *tree, Edges *edges)
{
    for(int i=0; i<edges->num_edges; ++i) {
        tree_add_edge(tree, &edges->edges[i]);
    }
}

static void tree_remove_edge(Tree *tree, Edge *e)
{
    tree->adj[e->source] = list_remove(tree->adj[e->source], e->dest);
    tree->adj[e->dest] = list_remove(tree->adj[e->dest], e->source);
}


static int tree_depth_search(Tree *tree, int source, int dest)
{
    assert(tree->visited[source]!=tree->cur_mark);
    if (source == dest)
    {
        return 1;
    }

    tree->visited[source] = tree->cur_mark;

    int found = 0;
    ListIt* a = list_iterator(tree->adj[source]);
    while(a != NULL && !found) {
        int node = list_iterator_get(a);
        if(tree->visited[node]!=tree->cur_mark) {
            found = tree_depth_search(tree, node, dest);
        }
        a = list_iterator_next(a);
    }

    return found;
}

static int tree_find_path(Tree *tree, int source, int dest)
{
    int result = tree_depth_search(tree, dest, source);
    tree->cur_mark++;

    return result;
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

static void read_edges(Edges* edges) {
    int num_edges = read_num_edges() - 1;
    for(int i=0; i<num_edges; ++i) {
        read_edge(&edges->edges[i]);
    }
    edges->num_edges = num_edges;
}

/// MARK: -------------------------   ALGORITHM   -------------------------

static int check_substitution(Tree* tree, Edge* candidate, Edge* query) {
    tree_remove_edge(tree, candidate);
    tree_add_edge(tree, query);

    int result = tree_find_path(tree, candidate->source, candidate->dest);

    tree_remove_edge(tree, query);
    tree_add_edge(tree, candidate);

    return result;
}

static int bin_search(Edges* edges, int weight) {
    int start = 0;
    int end = edges->num_edges-1;
    int mid;

    while( start <= end ){
        mid = (start + end)/2;
        if( edges->edges[mid].weight <= weight ) {
            start = mid+1;
            continue;
        }

        end = mid-1;
    }

    return start;
}


static int query_improve_graph(Tree* tree, Edges* edges, Edge* query) {
    if(tree->max_w <= query->weight) {
        return 0;
    }

    int first_candidate = bin_search(edges, query->weight);

    for(int i=first_candidate; i<edges->num_edges; ++i) {

        if (check_substitution(tree, &edges->edges[i], query))
        {
            return 1;
        }
    }

    return 0;
}

static int compare_edge(const Edge* e1, const Edge* e2) {
    return e1->weight - e2->weight;
}

/// MARK: -------------------------   DEBUG   -------------------------

static void print_edge(Edge e)
{
    printf("s:%d d:%d w:%d", e.source, e.dest, e.weight);
}

static void print_edges(Edges *edges, int start, int end)
{
    for (int i = start; i <= end; ++i)
    {
        print_edge(edges->edges[i]);
        printf("\n");
    }
}

static int max_list_len(Tree *tree)
{
    int max_len = 0;
    for (int i = 0; i < tree->size; ++i)
    {
        int cur_len = list_len(tree->adj[i]);
        if (max_len < cur_len)
        {
            max_len = cur_len;
        }
    }
    return max_len;
}

/// MARK: -------------------------   MAIN   -------------------------

int main(int argc, char** argv) {
    if(argc > 1) {
        FILE* filein = fopen(argv[1], "r");
        stdin = filein;
    }

    Tree tree;
    Edges edges;
    edges.num_edges=0;

    read_edges(&edges);

    qsort(edges.edges, edges.num_edges, sizeof(Edge), (int (*)(const void*, const void*)) compare_edge);

    tree_init(&tree, edges.num_edges+1);
    tree_build(&tree, &edges);

    int num_queries = read_num_edges();

    for(int i =0; i<num_queries; ++i) {
        Edge query;
        read_edge(&query);

        if(query_improve_graph(&tree, &edges, &query)) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }

    return 0;
}
