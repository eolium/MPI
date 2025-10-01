#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "kruskal.h"


void graph_free(graph_t* g){
    free(g->degrees);
    for (int i = 0; i < g->n; i++) {
        free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}


graph_t* read_graph(FILE* fp){
    int n;
    fscanf(fp, "%d", &n);
    int* degrees = malloc(n * sizeof(int));
    edge** adj = malloc(n * sizeof(edge*));
    for (int i = 0; i < n; i++){
        fscanf(fp, "%d", &degrees[i]);
        //printf("%d : %d\n", i, degrees[i]);
        adj[i] = malloc(degrees[i] * sizeof(edge));
        for (int j = 0; j < degrees[i]; j++) {
            edge e;
            e.x = i;
            fscanf(fp, " (%d, %f)", &e.y, &e.rho);
            adj[i][j] = e;
        }
    }
    graph_t* g = malloc(sizeof(graph_t));
    g->degrees = degrees;
    g->n = n;
    g->adj = adj;
    return g;
}


void print_graph(graph_t* g, FILE* fp){
    fprintf(fp, "%d\n", g->n);
    for (int i = 0; i < g->n; i++) {
        fprintf(fp, "%d ", g->degrees[i]);
        for (int j = 0; j < g->degrees[i]; j++) {
            edge e = g->adj[i][j];
            fprintf(fp, "(%d, %.3f) ", e.y, e.rho);
        }
        fprintf(fp, "\n");
    }
}


int number_of_edges(graph_t* g) {
    int out = 0;
    for (int i = 0; i < g->n; i++) {
        out += g->degrees[i];
    }

    return out / 2;
}


edge* get_edges(graph_t* g, int* nb_edges) {
    *nb_edges = number_of_edges(g);
    edge* T = malloc((*nb_edges) * sizeof(edge));
    int i = 0;
    
    for (int s = 0; s < g->n; s++) {
        for (int a = 0; a < g->degrees[s]; a++) {
            edge e = g->adj[s][a];
            if (e.x <= e.y) {
                T[i] = e;
                i++;
            }
        }
    }

    return T;
}

int comp(const void* x, const void* y) {
    if (((edge*) x)->rho < ((edge*) y)->rho) {
        return -1;
    } else {
        return 1;
    }
}

void sort_edges(edge* edges, int p) {
    qsort(edges, p, sizeof(edge), comp);
}

void print_edge_array(edge* edges, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d -> %d : %f\n", edges[i].x, edges[i].y, edges[i].rho);
    }
}

edge* kruskal(graph_t* g, int* nb_chosen) {
    int nb_edges;
    edge* A = get_edges(g, &nb_edges);

    edge* T = malloc((g->n-1) * sizeof(edge));
    int k = 0;

    sort_edges(A, nb_edges);

    partition_t* p = partition_new(g->n);


    for (int i = 0; i < nb_edges; i++) {
        if (p->nb_sets == 0) {
            break;
        }

        edge a = A[i];
        if (find(p, a.x) != find(p, a.y)) {
            merge(p, a.x, a.y);
            T[k] = a;
            k++;
        }
    }

    partition_free(p);

    free(A);

    *nb_chosen = k;
    return T;
}



int main(int argc, char** argv){
    FILE* f = fopen(argv[1], "r");
    graph_t* g = read_graph(f);
    fclose(f);
    printf("%d\n", number_of_edges(g));

    int nb_chosen;
    edge* magie = kruskal(g, &nb_chosen);

    for (int i = 0; i < nb_chosen; i++) {
        edge e = magie[i];
        printf("(%d -> %d, %f)\n", e.x, e.y, e.rho);
    }

    free(magie);

    graph_free(g);

    return 0;
}