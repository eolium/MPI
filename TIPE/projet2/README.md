# Nouveau Projet : Construction d'une librairie de Bigint et de Matrices


Je vais séparer le projet en plusieurs étapes :

1 - Créer un type bigint, avec des fonctions associées, pas optimisées :
    - new_bigint : int -> bigint
    - big_add : bigint * bigint -> bigint
    - big_mult : bigint * bigint -> bigint
    - big_div : bigint * bigint -> bigint
    - big_mod : bigint * bigint -> bigint
    - big_pow : bigint * int -> bigint


2 - Optimiser les fonctions suivantes :
    - mult : algo Karatsuba
    - pow : exp rapide et optimisation variables intermédiaires



3 - Créer un type matrix, avec des fonctions associées, pas optimisées :
    - new_matrix : int * int -> bigint
    - matrix_add : matrix * matrix -> matrix
    - matrix_lambda : matrix * lambda -> matrix
    - matrix_mult : matrix * matrix -> matrix
    - matrix_pow : matrix * int -> matrix





# 1 - Création du type Bigint

## A) Structure

On définit un bigint signé par un tableau d'entiers 32 bits (32 pour simplifier les multiplications plus tard) :

```C
struct bigint_s {
    bool signe;

    int taille;
    uint32_t* arr;
}
```