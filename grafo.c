// grafo.c

#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

graph new_graph(int n) {
    graph G;
    G.dim = n;
    G.nodes = (adj_list*)malloc(n * sizeof(adj_list));
    if (G.nodes == NULL) {
        fprintf(stderr, "Errore nell'allocazione della memoria\n");
        exit(1);
    }

    for (int i = 0; i < n; i++) {
        G.nodes[i] = NULL;  // inizializza liste vuote
    }

    return G;
}

void add_arc(graph* G, int u, int v, float w) {
    struct adj_node* t = (struct adj_node*)malloc(sizeof(struct adj_node));
    if (t == NULL) {
        fprintf(stderr, "Errore nell'allocazione dell'arco\n");
        exit(1);
    }

    t->node = v - 1;
    t->weight = w;
    t->next = G->nodes[u - 1];
    G->nodes[u - 1] = t;
}

void add_edge(graph* G, int u, int v, float w) {
    add_arc(G, u, v, w);
    add_arc(G, v, u, w);
}

int get_dim(graph g) {
    return g.dim;
}

adj_list get_adjlist(graph g, int u) {
    return g.nodes[u - 1];
}

int get_adjnode(struct adj_node* l) {
    return l->node + 1;
}

adj_list get_nextadj(adj_list l) {
    return l->next;
}

float get_adjweight(struct adj_node* l) {
    return l->weight;
}
