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

typedef struct _QueryNode {
    Edge* query;
    struct _QueryNode* next;
} QueryNode;

typedef struct _Tree {
    int visited[NUM_NODES];
    int active[NUM_NODES];
    Node** adj;
    QueryNode** endQueryNodes;
    int size;
    int max_w;
    int cur_mark;
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
    bzero(tree->visited, (tree->size+1) * sizeof(int));
    bzero(tree->active, (tree->size+1)*sizeof(int));
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

#define STACK_SIZE 100000

typedef struct _StackFrame {
    int source;
    int weight;
    ListIt* a;
} StackFrame;

typedef struct _Stack {
    StackFrame storage[STACK_SIZE];
    int top;
} Stack;

static void stack_init(Stack* stack) {
    stack->top = -1;
}

static int stack_empty(Stack* stack) {
    return stack->top < 0;
}

// static void stack_push(Stack* stack, int source, int weight, ListIt* a) {
//     ++stack->top;
//     assert(stack->top < STACK_SIZE);
//     assert(stack->top >= 0);
//     stack->storage[stack->top].source = source;
//     stack->storage[stack->top].weight = weight;
//     stack->storage[stack->top].a = a;
// }

#define stack_push(stack, src, wgt, adj) do { \
    ++(stack)->top; \
    (stack)->storage[(stack)->top].source = (src); \
    (stack)->storage[(stack)->top].weight = (wgt); \
    (stack)->storage[(stack)->top].a = (adj); \
    } while(0)

static StackFrame* stack_get(Stack* stack) {
    return &stack->storage[stack->top];
}

static void stack_pop(Stack* stack) {
    assert(stack->top >= 0);
    stack->top--;
}

int find_weight_for_source(Stack* stack, int source) {
    int cur_weight = -1;
    int top = stack->top;
    while(top>=0) {
        cur_weight = max(cur_weight, stack->storage[top].weight);
        if(stack->storage[top].source == source) {
            return cur_weight;
        }

        top--;
    }

    return -1;
}

static void update_queries(QueryNode** queries, Stack* stack, Tree* tree) {
    QueryNode** next_ptr = queries;
    QueryNode* query_it = *queries;

    while(query_it!=NULL) {
        if(!tree->active[query_it->query->source]) {
            next_ptr = &query_it->next;
            query_it = query_it->next;
            continue;
        }

        int weight = find_weight_for_source(stack, query_it->query->source);
        if(weight != -1) {
            query_it->query->answer = (query_it->query->weight < weight) ? YES : NO;

            // this is solved, unlinking query from query list, next_ptr will not move
            // since now it already point to the correct field of the previous query
            *next_ptr = query_it->next;
            query_it = query_it->next;
        } else {
            next_ptr = &query_it->next;
            query_it = query_it->next;
        }
    }
}

static void tree_depth_search(Tree *tree, Edge* orig, int source, int max_weight_found)
{
    assert(tree->visited[source]!=tree->cur_mark);

    Stack stack;
    stack_init(&stack);
    stack_push(&stack, source, -1, list_iterator(tree->adj[source]));
    tree->visited[source] = tree->cur_mark;
    tree->active[source] = 1;

    while(!stack_empty(&stack) && orig->answer==UNKNOWN) {
        StackFrame* frame = stack_get(&stack);
        if(frame->a == NULL) {
            tree->active[frame->source] = 0;
            stack_pop(&stack);
            continue;
        }

        int node = list_iterator_get(frame->a);
        if(tree->visited[node]!=tree->cur_mark) {
            int cur_weight = list_iterator_get_weight(frame->a);
            frame->a = list_iterator_next(frame->a);

            stack_push(&stack, node, cur_weight, list_iterator(tree->adj[node]));
            tree->visited[node] = tree->cur_mark;
            tree->active[node] = 1;

            // searching for possible queries to update
            update_queries(&tree->endQueryNodes[node], &stack, tree);
        } else {
            frame->a = list_iterator_next(frame->a);
        }
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

static void add_query_to_end_query_nodes(Edge* query, QueryNode** end_query_nodes) {
    QueryNode* qn = (QueryNode*) malloc(sizeof(QueryNode));
    qn->query = query;
    qn->next = end_query_nodes[query->dest];
    end_query_nodes[query->dest] = qn;
}

static void read_queries(Edges* queries, Tree* tree) {
    queries->num_edges = read_num_edges();
    queries->edges = (Edge**) malloc(sizeof(Edge*)*queries->num_edges);
    tree->endQueryNodes = (QueryNode **)calloc(tree->size + 1, sizeof(QueryNode *));

    for(int i=0; i<queries->num_edges; ++i) {
        queries->edges[i] = (Edge*) malloc(sizeof(Edge));
        read_edge(queries->edges[i]);
        queries->edges[i]->answer = UNKNOWN;
        add_query_to_end_query_nodes(queries->edges[i], tree->endQueryNodes);
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

        end = mid - 1;
    }

    return NULL;
}


static void query_improve_graph(Tree* tree, Edge* query) {
    if(query->weight >= tree->max_w) {
        query->answer = NO;
        return ;
    }

    tree_depth_search(tree, query, query->source, -1);
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
    Edges queries;

    read_edges(&tree);
    read_queries(&queries, &tree);
    
    for(int i = 0; i < queries.num_edges; ++i) {
        if(queries.edges[i]->answer != UNKNOWN) {
            print_answer(queries.edges[i]->answer);
            continue;
        }

        query_improve_graph(&tree, queries.edges[i]);
        print_answer(queries.edges[i]->answer);
    }

    return 0;
}
