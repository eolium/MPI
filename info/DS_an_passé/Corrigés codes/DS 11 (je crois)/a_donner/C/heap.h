#ifndef HEAP_H
#define HEAP_H

#include <stdbool.h>

#include "graph.h"


typedef struct heap_t heap_t;

// Création d'une file vide pouvant contenir *capacity* arêtes
heap_t *heap_create(int capacity);

// Libération des ressources associées à une file
void heap_free(heap_t *heap);

// Détermine si la file est vide
bool heap_is_empty(heap_t *heap);

// Ajoute une arête à la file.
void heap_push(heap_t *heap, edge_t pair);

// Renvoie l'arête de poids minimal présente dans la file,
// sans modifier la file.
// Erreur si la file est vide.
edge_t heap_get_min(heap_t *heap);

// Renvoie l'arête de poids minimal présente dans la file,
// et la supprime de la file.
// Erreur si la file est vide.
edge_t heap_extract_min(heap_t *heap);



// Renvoie la capacité de la file
int heap_get_capacity(heap_t *heap);

void heap_print(heap_t *heap);

#endif
