#include <stdlib.h>
#include <stdio.h>

#include "graph.h"

void graph_free(graph_t *g){
    free(g->degrees);
    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}

graph_t *read_graph(FILE *fp){
    int n;
    fscanf(fp, "%d", &n);
    int p = 0;
    int *degrees = malloc(n * sizeof(int));
    edge_t **adj = malloc(n * sizeof(edge_t*));
    for (int i = 0; i < n; i++){
        fscanf(fp, "%d", &degrees[i]);
        p += degrees[i];
        /* printf("%d : %d\n", i, degrees[i]); */
        adj[i] = malloc(degrees[i] * sizeof(edge_t));
        for (int j = 0; j < degrees[i]; j++) {
            edge_t e;
            e.x = i;
            fscanf(fp, " (%d, %lf)", &e.y, &e.weight);
            adj[i][j] = e;
        }
    }
    graph_t *g = malloc(sizeof(graph_t));
    g->degrees = degrees;
    g->n = n;
    g->p = p;
    g->adj = adj;
    return g;
}

void print_graph(graph_t *g, FILE *fp){
    fprintf(fp, "%d\n", g->n);
    for (int i = 0; i < g->n; i++) {
        fprintf(fp, "%d ", g->degrees[i]);
        for (int j = 0; j < g->degrees[i]; j++) {
            edge_t e = g->adj[i][j];
            fprintf(fp, "(%d, %.3f) ", e.y, e.weight);
        }
        fprintf(fp, "\n");
    }
}

void print_edge_array(edge_t *edges, int len){
    for (int i = 0; i < len; i++) {
        edge_t e = edges[i];
        printf("%d %d %.2f\n", e.x, e.y, e.weight);
    }
    printf("\n");
}
