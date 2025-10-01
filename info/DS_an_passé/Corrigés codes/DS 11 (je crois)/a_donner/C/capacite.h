#ifndef CAPA_H
#define CAPA_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h> //INFINITY
#include <string.h>

#include "heap.h"
#include "graph.h"
#include "corrige.h"

int nb_edges(graph_t* g);

double** matrice_initiale(graph_t* g);

double** copy_matrice(double** m, int n);

double capacite_max(graph_t* g, int x, int y);

edge_t *prim(graph_t *g);

#endif