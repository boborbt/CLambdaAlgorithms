#include <stdio.h>
#include <stdlib.h>

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
    int fwd[NUM_NODES];
    int bck[NUM_NODES];
    int size;
} Tree;

void print_edge(Edge e)
{
    printf("s:%d d:%d w:%d", e.source, e.dest, e.weight);
}

void print_edges(Edges *edges, int start, int end)
{
    for (int i = start; i <= end; ++i)
    {
        print_edge(edges->edges[i]);
        printf("\n");
    }
}

void tree_init(Tree* tree, int num_nodes) {
    for(int i=0; i<num_nodes+1; ++i) {
        tree->fwd[i] = i;
        tree->bck[i] = i;
    }

    tree->size = num_nodes;
}

void tree_add_edge(Tree* tree, Edge* e) {
    int i = e->source;
    int j = e->dest;

    if(i < j) {
        tree->fwd[i] = j;
        tree->bck[j] = i;
    } else {
        tree->fwd[j] = i;
        tree->bck[i] = j;
    }
}

void tree_build(Tree* tree, Edges* edges) {
    for(int i=0; i<edges->num_edges; ++i) {
        tree_add_edge(tree, &edges->edges[i]);
    }
}

void tree_remove_edge(Tree* tree, Edge* e) {
    int i = e->source;
    int j = e->dest;

    if(i < j) {
        tree->fwd[i] = i;
        tree->bck[j] = j;
    } else {
        tree->fwd[j] = j;
        tree->bck[i] = i;
    }
}

int tree_depth_search(Tree *tree, short *visited, int source, int dest)
{
    if (source == dest)
    {
        return 1;
    }

    int found = 0;
    visited[source] = 1;

    if (visited[tree->fwd[source]] == 0)
    {
        found = tree_depth_search(tree, visited, tree->fwd[source], dest);
    }
    if (!found && visited[tree->bck[source]] == 0)
    {
        found = tree_depth_search(tree, visited, tree->bck[source], dest);
    }

    return found;
}

int tree_find_path(Tree* tree, int source, int dest) {
    short* visited = (short*) calloc(tree->size+1, sizeof(short));
    int result = tree_depth_search(tree, visited, source, dest);
    free(visited);

    return result;
}




static int read_num_edges() {
    int result;
    scanf("%d", &result);
    return result;
}


static void read_edge(Edge* edge) {
    scanf("%d %d %d\n", &edge->source, &edge->dest, &edge->weight);
}

static void read_edges(Edges* edges) {
    int num_edges = read_num_edges() - 1;
    for(int i=0; i<num_edges; ++i) {
        read_edge(&edges->edges[i]);
    }
    edges->num_edges = num_edges;
}

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

    printf("fcs: %d\n", first_candidate);
    print_edges(edges, first_candidate-2, first_candidate+2 );

    // for(int i=0; i<edges->num_edges; ++i) {
    //     printf("%d w:%d\n", i, edges->edges[i].weight);
    // }
    // printf("query weight: %d first candidate: %d\n", query->weight, first_candidate);

    for(int i=first_candidate; i<edges->num_edges; ++i) {
        if(check_substitution(tree, &edges->edges[i], query)) {
            return 1;
        }
    }

    return 0;
}

static int compare_edge(const Edge* e1, const Edge* e2) {
    return e1->weight - e2->weight;
}


int main() {
    Tree tree;
    Edges edges;
    edges.num_edges=0;

    read_edges(&edges);
    qsort(edges.edges, edges.num_edges, sizeof(Edge), (int (*)(const void*, const void*)) compare_edge);

    print_edges(&edges, 0, 10);
    printf("num edges:%d\n", edges.num_edges);

    tree_init(&tree, edges.num_edges+1);
    tree_build(&tree, &edges);

    int num_queries = read_num_edges();
    for(int i =0; i<num_queries; ++i) {
        Edge query;
        read_edge(&query);

        if(i<226 || i>226) {
            continue;
        }

        printf("trying query:");print_edge(query);printf("\n");

        if(query_improve_graph(&tree, &edges, &query)) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }

    return 0;
}