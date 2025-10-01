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

double capacite_max_cor(graph_t* g, int x, int y);

edge_t *prim_cor(graph_t *g);

edge_t* arbre_max_cor(graph_t* g);

double cmax_cor(graph_t* g, int x, int y);

#endif
