
#include "capacite.h"


int nb_edges(graph_t* g){
    //TODO
}

double** matrice_initiale(graph_t* g){
    //TODO
}


double** copy_matrice(double** m, int n){
    //TODO
}


double capacite_max(graph_t* g, int x, int y) {
    //TODO
}


edge_t *prim(graph_t *g){
    //TODO
}


int main(int argc, char* argv[]){
    FILE* fptr = fopen("g1.txt", "r");
    graph_t* g1 = read_graph(fptr);
    fclose(fptr);

    //A vous de jouer !
    printf("\nVos tests ici...\n");


    graph_free(g1);
    
    return EXIT_SUCCESS;
}
