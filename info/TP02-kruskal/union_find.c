#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "union_find.h"
#include <assert.h>

int nb_sets(partition_t *p){
    return p->nb_sets;
}

int nb_elements(partition_t *p){
    return p->nb_elements;
}

partition_t *partition_new(int nb_elements) {
    partition_t* melodie = malloc(sizeof(partition_t));

    melodie->nb_sets = nb_elements;
    melodie->nb_elements = nb_elements;
    melodie->arr = malloc(nb_elements * sizeof(int));

    for (int i = 0; i < nb_elements; i++) {
        melodie->arr[i] = -1;
    }

    return melodie;
}

void partition_free(partition_t *p) {
    free(p->arr);
    free(p);
}

element find(partition_t *p, element x) {
    if (p->arr[x] < 0) {
        return x;
    }

    int repr = find(p, p->arr[x]);

    p->arr[x] = repr;

    return repr;
}

void print_partition(partition_t* p) {
    for (int i = 0; i < p->nb_elements; i++) {
        printf("%d ", p->arr[i]);
    }
    printf("\n");
}

void merge(partition_t *p, element x, element y) {
    element rx = find(p, x);
    element ry = find(p, y);
    
    if (rx == ry) {
        return;
    }

    assert(p->arr[rx] <= 0 && p->arr[ry] <= 0);

    if (abs(p->arr[rx]) >= abs(p->arr[ry])) {
        p->arr[ry] += p->arr[rx];
        p->arr[rx] = ry;
    } else {
        p->arr[rx] += p->arr[ry];
        p->arr[ry] = rx;
    }

    p->nb_sets--;
}


/*int main() {
    int taille = 10;

    partition_t* p = partition_new(taille);

    merge(p, 0, 1);
    merge(p, 1, 2);
    merge(p, 3, 4);
    merge(p, 0, 3);

    for (int i = 0; i < taille; i++) {
        printf("%d ", find(p, i));
    }
    printf("\n");

    for (int i = 0; i < taille; i++) {
        printf("%d ", p->arr[i]);
    }
    printf("\n");
}*/