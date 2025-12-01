#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "afdio.h"

etat transition(afd a, etat q, lettre l) {
    return a.delta[q][l];
}

void acceptants(afd a) {
    for (int i = 0; i < a.n; i++) {
        if (a.term[i]) {
            printf("%d ", i);
        }
    }
    printf("\n");
}

int** copie_matrice(int** mat, int n, int m) {
    int** out = malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {
        out[i] = copie_tableau(mat[i], m);
    }

    return out;
}

afd complementaire(afd a) {
    
}


int main() {
    printf("%d\n", transition(afd_exemple_a1(), 1, 0));

    acceptants(afd_exemple_a1());
}