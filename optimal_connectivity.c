#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

typedef struct _Tree {
    int* adj;
    int size;
} Tree;

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

/// MARK: -------------------------   TREE   -------------------------
static void tree_init(Tree* tree, int num_nodes) {
    tree->adj = (int *)calloc((num_nodes + 1) * (num_nodes + 1), sizeof(int));
    tree->size = num_nodes;
}

#define AT(s,a) (s)*((tree)->size+1)+(a)
#define tree_at(tree, s, a) (tree)->adj[(s) * ((tree)->size + 1) + (a)]

static void tree_set_adj(Tree *tree, int s, int a, int new_adj)
{
    assert(AT(s,a) < TREE_TAB_SIZE);

    tree_at(tree,s,a) = new_adj;
}

static void tree_add_edge(Tree *tree, Edge *e)
{
    int a=0;
    while(tree_at(tree,e->source,a)!=0) { ++a; }
    tree_set_adj(tree,e->source,a, e->dest);

    a = 0;
    while(tree_at(tree,e->dest,a)!=0) { ++a; }
    tree_set_adj(tree,e->dest,a, e->source);
}

static void tree_build(Tree *tree, Edges *edges)
{
    for(int i=0; i<edges->num_edges; ++i) {
        tree_add_edge(tree, &edges->edges[i]);
    }
}

static void tree_remove_edge(Tree *tree, Edge *e)
{
    int a=0;
    while(tree_at(tree, e->source,a)!=e->dest) { ++a; }
    while(tree_at(tree, e->source,a)!=0) {
        tree_set_adj(tree,e->source,a, tree_at(tree,e->source,a+1));
        a++;
    }
}

static int tree_depth_search(Tree *tree, short *visited, int source, int dest)
{
    assert(!visited[source]);
    if (source == dest)
    {
        return 1;
    }

    visited[source] = 1;

    int found = 0;
    int a=0;
    while(tree_at(tree,source,a)!=0 && !found) {
        if(!visited[tree_at(tree,source,a)]) {
            found = tree_depth_search(tree, visited, tree_at(tree,source,a), dest);
        }
        a++;
    }

    return found;
}

static int tree_find_path(Tree *tree, int source, int dest)
{
    short* visited = (short*) calloc(tree->size+1, sizeof(short));
    int result = tree_depth_search(tree, visited, source, dest);
    free(visited);

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
    int first_candidate = bin_search(edges, query->weight);

    // printf("fcs: %d\n", first_candidate);
    // print_edges(edges, first_candidate-2, first_candidate+2 );

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

/// MARK: -------------------------   MAIN   -------------------------

int main() {
    Tree tree;
    Edges edges;
    edges.num_edgesd=0;

    read_edges(&edges);

    // print_edges(&edges, 0, 4);
    // printf("num edges:%d\n", edges.num_edges);

    qsort(edges.edges, edges.num_edges, sizeof(Edge), (int (*)(const void*, const void*)) compare_edge);

    tree_init(&tree, edges.num_edges+1);
    tree_build(&tree, &edges);

    int num_queries = read_num_edges();

    // printf("num queries:%d\n", num_queries );
    for(int i =0; i<num_queries; ++i) {
        Edge query;
        read_edge(&query);

        // print_edge(query);
        // printf("\n");

    //     if(i<3 || i>4) {
    //         continue;
    //     }

    //     printf("trying query:");print_edge(query);printf("\n");

        if(query_improve_graph(&tree, &edges, &query)) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }

    return 0;
}
