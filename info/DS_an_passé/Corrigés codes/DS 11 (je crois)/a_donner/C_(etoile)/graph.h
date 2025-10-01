#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include <stdio.h>

typedef double weight_t;

struct edge {
    int x;
    int y;
    double weight;
};
typedef struct edge edge_t;

struct graph {
    int n;
    int p;
    int* degrees;
    edge_t** adj;
};
typedef struct graph graph_t;


void graph_free(graph_t *g);

graph_t *read_graph(FILE *fp);

void print_graph(graph_t *g, FILE *fp);

void print_edge_array(edge_t *edges, int len);

#endif
