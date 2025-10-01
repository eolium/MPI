#ifndef COR_H
#define COR_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h> // INFINITY
#include <string.h>

#include "heap.h"
#include "graph.h"


int nb_edges_cor(graph_t* g);

double** matrice_initiale_cor(graph_t* g);

double** copy_matrice_cor(double** m, int n);

double capacite_max_cor(graph_t* g, int x, int y);

edge_t *prim_cor(graph_t *g);

#endif
