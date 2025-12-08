#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

bigint new_bigint(int n) {
    bool signe;
    if (n >= 0) {
        signe = true;
    } else {
        signe = false;
    }

    int taille = 1;

    uint32_t* arr = malloc(sizeof(uint32_t));
}

void free_bigint(bigint A) {
    free(A.arr);
}

bigint print_bigint(bigint A) {
    
}