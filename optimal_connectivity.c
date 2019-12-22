#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <strings.h>
#include <time.h>

#define NUM_NODES 100001

typedef enum {
    UNKNOWN=0,
    YES,
    NO
} Answer;

typedef struct _Edge {
    int source;
    int dest;
    int weight;
    Answer answer;
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
    int visited[NUM_NODES];
    Node** adj;
    int size;
    int max_w;
    int cur_mark;
} Tree;


typedef Node ListIt;

#define max(a,b) (a) > (b) ? (a) : (b)
static Edge *bin_search(Edges *edges, Edge *goal);
static int compare_edges(Edge ** e1, Edge** e2);

    /// MARK: -------------------------   LIST   -------------------------

    static int list_len(Node *list)
{
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
    bzero(tree->visited, tree->size * sizeof(int));
    tree->cur_mark = 1;
}

static void tree_add_edge(Tree *tree, Edge *e)
{
    tree->adj[e->source] = list_add(tree->adj[e->source], e->dest, e->weight);
    tree->adj[e->dest] = list_add(tree->adj[e->dest], e->source, e->weight);
    tree->max_w = max(tree->max_w, e->weight);
}

static void tree_remove_edge(Tree *tree, Edge *e)
{
    tree->adj[e->source] = list_remove(tree->adj[e->source], e->dest);
    tree->adj[e->dest] = list_remove(tree->adj[e->dest], e->source);
}

static void update_query(Edge* query, int max_weight_found) {
    if(query == NULL) {
        return;
    }

    if (query->weight < max_weight_found) {
        query->answer = YES;
    }
    else {
        query->answer = NO;
    }
}

static void tree_depth_search(Tree *tree, Edge* orig, int source, int max_weight_found, Edges* sorted_queries)
{
    assert(tree->visited[source]!=tree->cur_mark);

    tree->visited[source] = tree->cur_mark;

    ListIt* a = list_iterator(tree->adj[source]);
    while(a != NULL && orig->answer==UNKNOWN) {
        int node = list_iterator_get(a);

        if(tree->visited[node]!=tree->cur_mark) {
            int cur_weight = list_iterator_get_weight(a);
            int hop_max_weight = max(max_weight_found, cur_weight);

            Edge edge_n;
            edge_n.source = orig->source;
            edge_n.dest = node;

            // Edge* query = bin_search(sorted_queries, &edge_n);
            Edge* query = NULL;
            if(edge_n.source == orig->source && edge_n.dest == orig->dest) {
                query = orig;
                update_query(query, hop_max_weight);
                return;
            }
            // update_query(query, hop_max_weight); 

            tree_depth_search(tree, orig, node, hop_max_weight, sorted_queries);
        }
        a = list_iterator_next(a);
    }
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

static void read_queries(Edges* queries) {
    queries->num_edges = read_num_edges();
    queries->edges = (Edge**) malloc(sizeof(Edge*)*queries->num_edges);

    for(int i=0; i<queries->num_edges; ++i) {
        queries->edges[i] = (Edge*) malloc(sizeof(Edge));
        read_edge(queries->edges[i]);
        queries->edges[i]->answer = UNKNOWN;
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

// static int check_substitution(Tree* tree, Edge* candidate, Edge* query) {
//     tree_remove_edge(tree, candidate);
//     tree_add_edge(tree, query);

//     int result = tree_find_path(tree, candidate->source, candidate->dest);

//     tree_remove_edge(tree, query);
//     tree_add_edge(tree, candidate);

//     return result;
// }

static Edge* bin_search(Edges* edges, Edge* goal) {
    int start = 0;
    int end = edges->num_edges-1;
    int mid;

    while( start <= end ){
        mid = (start + end)/2;

        if(compare_edges(&edges->edges[mid], &goal) == 0) {
            return edges->edges[mid];
        }

        if( compare_edges(&edges->edges[mid], &goal) < 0) {
            start = mid+1;
            continue;
        }

        end = mid-1;
    }

    return NULL;
}


static void query_improve_graph(Tree* tree, Edge* query, Edges* sorted_queries) {
    tree_depth_search(tree, query, query->source, -1, sorted_queries);
    tree->cur_mark++;
}

static int compare_edges(Edge **e1, Edge **e2)
{
    if((*e1)->source < (*e2)->source) {
        return -1;
    }
    
    if((*e1)->source > (*e2)->source) {
        return 1;
    }
    
    if((*e1)->dest < (*e2)->dest) {
        return -1;
    }
    
    if((*e1)->dest > (*e2)->dest) {
        return 1;
    }

    return 0;
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
        print_edge(*edges->edges[i]);
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
    read_edges(&tree);

    float longest = 0;
    int longest_index=-1;

    Edges queries;
    read_queries(&queries);
    Edges sorted_queries = queries;
    sorted_queries.edges = (Edge**) malloc(sizeof(Edge*)*queries.num_edges);
    memcpy(sorted_queries.edges, queries.edges, sizeof(Edge*) * queries.num_edges);
    qsort(sorted_queries.edges, sorted_queries.num_edges, sizeof(Edge *), (int (*)(const void *, const void *))compare_edges);

    for(int i=0; i<queries.num_edges; ++i) {
        if(queries.edges[i]->answer != UNKNOWN) {
            print_answer(queries.edges[i]->answer);
            continue;
        }

        query_improve_graph(&tree, queries.edges[i], &sorted_queries);
        print_answer(queries.edges[i]->answer);
    }
    

    // for(int i =0; i<num_queries; ++i) {
    //     Edge query;
    //     read_edge(&query);

    //     clock_t start = clock();
    //     if (query_improve_graph(&tree, &query)) {
    //         printf("YES\n");
    //     }
    //     else {
    //         printf("NO\n");
    //     }
    //     clock_t end = clock();

    //     if((end - start)/(float)CLOCKS_PER_SEC > longest) {
    //         longest = (end - start)/(float)CLOCKS_PER_SEC;
    //         longest_index = i;
    //     }
    // }

    // printf("longest: %f index: %d\n", longest, longest_index);

    return 0;
}
