

// grafo.h

#ifndef GRAFO_H
#define GRAFO_H

typedef struct adj_node {
  int node;           
  float weight;       
  struct adj_node* next;  
} adj_node;

typedef adj_node* adj_list;

typedef struct {
  int dim;            
  adj_list* nodes;    
} graph;

// Prototipi delle funzioni
graph new_graph(int n);
void add_arc(graph* G, int u, int v, float w);
void add_edge(graph* G, int u, int v, float w);
int get_dim(graph g);
adj_list get_adjlist(graph g, int u);
int get_adjnode(struct adj_node* l);
adj_list get_nextadj(adj_list l);
float get_adjweight(struct adj_node* l);

#endif
