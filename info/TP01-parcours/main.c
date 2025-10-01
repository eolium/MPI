#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "dynarray.h"
#include "pile.h"
#include "file.h"
#include "graphes.h"



int main() {
  graphe G = cree_graphe_exemple();

  graphe_print(G);

  int* ordre = malloc(G.n * sizeof(int));
  graphe_bfs_complet(G, ordre);

  for (int i = 0; i < G.n; i++) {
    printf("%d ", ordre[i]);
  }

  printf("\n\n");

  graphe_dfs_s0_pile(G, 0, ordre);
  
  for (int i = 0; i < G.n; i++) {
    printf("%d ", ordre[i]);
  }
  printf("\n");

  int* ouverture = malloc(G.n * sizeof(int));
  int* fermeture = malloc(G.n * sizeof(int));

  graphe_dfs_s0_rec(G, 0, ouverture, fermeture);

  for (int i = 0; i < G.n; i++) {
    printf("%d\t", ouverture[i]);
  }
  printf("\n");

  for (int i = 0; i < G.n; i++) {
    printf("%d\t", fermeture[i]);
  }
  printf("\n");

  free(ouverture);
  free(fermeture);
  free(ordre);
  graphe_free(G);
}
