#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

struct bigint_s {
    bool signe;

    int taille;
    uint32_t* arr;
}

typedef bigint bigint_s;


bigint new_bigint(int n);

void free_bigin(bigint A);

void print_bigint(bigint A);

bigint big_add(bigint A, bigint B);

bigint big_mult(bigint A, bigint B);

bigint big_div(bigint A, bigint B);

bigint big_mod(bigint A, bigint B);

bigint big_pow(bigint A, bigint B);