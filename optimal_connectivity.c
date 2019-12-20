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

typedef struct _UnionFind {
    int sets[NUM_NODES];
    int ranks[NUM_NODES];
} UnionFind;

static void UF_init(UnionFind* uf, int size) {
    for(int i=0; i<size+1; ++i) {
        uf->sets[i] = i;
        uf->ranks[i] = 0;
    }
}

static void UF_link(UnionFind* uf, int i, int j) {
    uf->sets[j] = i;
}

static int UF_find(UnionFind* uf, int i) {
    if(uf->sets[i] == i) {
        return i;
    }

    int p_i = UF_find(uf, uf->sets[i]);
    uf->sets[i] = p_i;

    return p_i;
}

static int UF_union(UnionFind* uf, int i, int j) {
    // printf("joining %d %d\n", i, j);
    // printf("uf: ");
    // for(int i=0; i<=6; ++i) {
    //     printf("%d:%d ", i, uf->sets[i]);
    // }
    // printf("\n");

    int p_i = UF_find(uf, i);
    int p_j = UF_find(uf, j);
    if(p_i==p_j) {
        // printf("can't join\n");
        return 0;
    }

    if(uf->ranks[p_i] > uf->ranks[p_j]) {
        UF_link(uf, p_j, p_i);
    } else if(uf->ranks[p_i] < uf->ranks[p_j]) {
        UF_link(uf, p_i, p_j);
    } else {
        UF_link(uf, p_i, p_j);
        uf->ranks[p_i]++;
    }

    return 1;
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

static int check_substitution(Edges* edges, Edge* candidate, Edge* query) {
    UnionFind uf;
    UF_init(&uf, edges->num_edges+1);
    UF_union(&uf, query->source, query->dest);

    for(int i=0; i<edges->num_edges; ++i) {
        if(&edges->edges[i] == candidate) {
            // the candidate edge is not in the candidate tree, we skip it
            continue;
        }

        if(!UF_union(&uf, edges->edges[i].source, edges->edges[i].dest )) {
            return 0;
        }
    }

    return 1;
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

static int query_improve_graph(Edges* edges, Edge* query) {
    int first_candidate = bin_search(edges, query->weight);

    // for(int i=0; i<edges->num_edges; ++i) {
    //     printf("%d w:%d\n", i, edges->edges[i].weight);
    // }
    // printf("query weight: %d first candidate: %d\n", query->weight, first_candidate);

    for(int i=first_candidate; i<edges->num_edges; ++i) {
        if(check_substitution(edges, &edges->edges[i], query)) {
            return 1;
        }
    }

    return 0;
}

static int compare_edge(const Edge* e1, const Edge* e2) {
    return e1->weight - e2->weight;
}


int main() {
    Edges edges;
    edges.num_edges=0;

    read_edges(&edges);
    qsort(edges.edges, edges.num_edges, sizeof(Edge), (int (*)(const void*, const void*)) compare_edge);


    int num_queries = read_num_edges();
    for(int i =0; i<num_queries; ++i) {
        Edge query;
        read_edge(&query);
        if(query_improve_graph(&edges, &query)) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }

    return 0;
}