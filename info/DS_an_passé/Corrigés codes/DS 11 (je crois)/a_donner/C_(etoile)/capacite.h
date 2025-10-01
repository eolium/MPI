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

double capacite_max(graph_t* g, int x, int y);

edge_t *prim(graph_t *g);

edge_t* arbre_max(graph_t* g);

double cmax(graph_t* g, int x, int y);

#endif