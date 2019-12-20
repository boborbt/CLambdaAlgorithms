#include <stdio.h>
#include <stdlib.h>

typedef struct _Edge {
    int source;
    int dest;
    int weight;
} Edge;

typedef struct _Edges {
    Edge edges[10000];
    int counts[10000];
    int total_count;
    int num_edges;
} Edges;

static int read_num_edges() {
    int result;
    scanf("%d", &result);
    return result;
}

void read_edge(Edge* edge) {
    scanf("%d %d %d\n", &edge->source, &edge->dest, &edge->weight);
}

static void read_edges(Edges* edges) {
    int num_edges = read_num_edges() - 1;
    for(int i=0; i<num_edges; ++i) {
        read_edge(&edges->edges[i]);
        printf("read edge: %d %d %d\n", edges->edges[i].source, edges->edges[i].dest, edges->edges[i].weight);
    }
    edges->num_edges = num_edges;
}

int check_substitution(Edges* edges, Edge* candidate, Edge* query) {
    int source_count = 
}

int query_improve_graph(Edges* edges, Edge* query) {
    int first_candidate = bin_search(edges, query->weight);
    for(int i=first_candidate; i<edges->num_edges; ++i) {
        if(check_substitution(edges, &edges->edges[i], query)) {
            return 1;
        }
    }

    return 0;
}

void update_counts(Edges* edges) {
    int tot = 0;
    for(int i=0; i<edges->num_edges+1; ++i) {
        edges->counts[i] = 0;
    }

    for(int i=0; i<edges->num_edges; ++i) {
        edges->counts[edges->edges[i].source]++;
        edges->counts[edges->edges[i].dest]++;
        tot+=2;
    }

    edges->total_count = tot;
}


int main() {
    Edges edges;
    edges.num_edges=0;

    read_edges(&edges);
    update_counts(&edges);

    for(int i=0; i<edges.num_edges+2; ++i) {
        printf("count[%d]: %d\n", i, edges.counts[i]);
    }
    printf("total count:%d\n", edges.total_count);


    int num_queries = read_num_edges();
    printf("num queries: %d\n", num_queries);
    for(int i =0; i<num_queries; ++i) {
        Edge query;
        read_edge(&query);
        printf("testing query: %d %d %d\n", query.source, query.dest, query.weight);
        if(query_improve_graph(&edges, &query)) {
            printf("YES\n");
        } else {
            printf("NO\n");
        }
    }

    return 0;
}