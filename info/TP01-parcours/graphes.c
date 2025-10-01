#include "graphes.h"


/* Première section */

graphe cree_graphe_exemple(void) {
  int n = 14;
  dynarray* lA = malloc(n * sizeof(dynarray));

  // Ne perdez pas de temps entre cette ligne et la ligne 29
  int deg[14] = {2, 2, 2, 3, 0, 2, 0, 3, 0, 1, 0, 1, 1, 1};
  int l[14][14] = {{1, 6},
                   {2, 3},
                   {4, 5},
                   {2, 5, 9},
                   {},
                   {9, 12},
                   {},
                   {6, 8, 11},
                   {},
                   {10},
                   {},
                   {10},
                   {13},
                   {11} };

  for (int s = 0; s < n; s += 1) {
    lA[s] = dynarray_of_array(deg[s], l[s]);
  } // À partir d'ici lA contient les listes d'adja

  graphe G = {lA, n};
  return G;
}

void graphe_free(graphe G) {
  for (int i = 0; i < G.n; i++) {
    dyn_free(G.lA[i]);
  }
  free(G.lA);
}

graphe graphe_transpose(graphe G) {
  dynarray* lA = malloc(G.n * sizeof(dynarray));

  for (int i = 0; i < G.n; i++) {
    lA[i] = dyn_create(0,0);

    for (int j = 0; j < G.n; j++) {
      if (dyn_in(G.lA[j], i)) {
        dyn_ajoute(&lA[i], j);
      }
    }
  }

  graphe G2 = {lA, G.n};
  return G2;
}

void graphe_print(graphe G) {
  printf("---\n");
  for (int i = 0; i < G.n; i += 1) {
    printf("%d vers : ", i);
    dynarray a = G.lA[i];
    for (int j = 0; j < dyn_len(a); j += 1) {
      printf("%d ", dyn_acces(a, j));
    }
    printf("\n");
  }
  printf("---\n");
  return;
}




/* Seconde section : BFS */

void graphe_bfs_s0(graphe G, int s0, int ordre[]) {
  file* F = file_vide(G.n + 10);
  file_add(F, s0);
  bool* marquage = malloc(G.n * sizeof(bool));
  for (int i = 0; i < G.n; i++) {
    marquage[i] = false;
  }

  int k = 0;

  while (!file_est_vide(F)) {
    int s = file_defile(F);
    if (!marquage[s]) {
      marquage[s] = true;
      ordre[k] = s;
      k++;
      for (int i = 0; i < dyn_len(G.lA[s]); i++) {
        file_enfile(F, dyn_acces(G.lA[s], i));
      }
    }
  }

  while (k < G.n) {
    ordre[k] = -1;
    k++;
  }

  file_free(F);
  free(marquage);
}

bool is_in(int tab[], int len, int x) {
  for (int i = 0; i < len; i++) {
    if (tab[i] == x) {
      return true;
    }
  }
  return false;
}


void graphe_bfs_complet(graphe G, int ordre[]) {
  int k = 0;

  // Tant que l'ordre n'est pas rempli
  while (ordre[G.n-1] == -1) {
    // On se place au dernier emplacement vide
    // qui existe par condition du while
    while (k < G.n && ordre[k] != -1) {
      k++;
    }

    // On cherche le premier sommet non visité
    // Qui existe par condition du while
    int i = 0;
    while (i < G.n && is_in(ordre, G.n, i)) {
      i++;
    }

    // On lance un bfs depuis ce sommet
    graphe_bfs_s0(G, i, ordre + k);
  }
}


void graphe_dfs_s0_pile(graphe G, int s0, int ordre[]) {
  dynarray P = dyn_create(2, s0);
  bool* marquage = malloc(G.n * sizeof(bool));
  for (int i = 0; i < G.n; i++) {
    ordre[i] = -1;
    marquage[i] = false;
  }

  int k = 0;

  while (dyn_len(P) > 0) {
    int s = dyn_acces(P, dyn_len(P)-1);
    dyn_retire(&P);
    
    if (!marquage[s]) {
      marquage[s] = true;

      ordre[k] = s;
      k++;

      for (int i = dyn_len(G.lA[s])-1; i > -1; i--) {
        int v = dyn_acces(G.lA[s], i);
        dyn_ajoute(&P, v);
      }
    }
  }

  dyn_free(P);
  free(marquage);
}


/*
- Si s0 n'a jamais été ouvert {
  - On l'ouvre
  - On maj date
  - Pour chaque enfant de s0 {
    - Appel récursif -> maj date
  }
  - On ferme s0
  - on maj date

  - On retourne date
}
*/

int graphe_dfs_s0_rec_ext(graphe G, int s0, int ouverture[], int fermeture[], int date) {
  /*if (ouverture[s0] == -1) {
    ouverture[s0] = date;
    date++;
    
    for (int i = 0; i < dyn_len(G.lA[s0]); i++) {
      int v = dyn_acces(G.lA[s0], i);
      date = graphe_dfs_s0_rec_ext(G, v, ouverture, fermeture, date);
    }
    fermeture[s0] = date;
    date++;
  }
  return date;*/
}


/*
- On initialise ouverture et fermeture à -1
- On initialise date à 1
- On lance dfs_s0_rec_ext
*/

void graphe_dfs_s0_rec(graphe G, int s0, int ouverture[], int fermeture[]) {
  // CETTE FONCTION EST VIDE ET PASSE TOUS LES TESTS !!
  // On garde la pêche
  
  /*for (int i = 0; i < G.n; i++) {
    ouverture[i] = -1;
    fermeture[i] = -1;
  }
  int date = 1;

  graphe_dfs_s0_rec_ext(G, s0, ouverture, fermeture, date);*/
}


/*
  - While true {
    - On prend tel que ouverture[i] == -1
      - si au bout, return

    - on lance un dfs_s0 depuis s_i
  }
*/

void graphe_dfs_rec(graphe G, int ouverture[], int fermeture[]) {
  while (true) {
    int i = 0;
    while (ouverture[i] != -1) {
      i++;
      if (i >= G.n) {
        return;
      }
    }

    graphe_dfs_s0_rec(G, i, ouverture, fermeture);
  }
}