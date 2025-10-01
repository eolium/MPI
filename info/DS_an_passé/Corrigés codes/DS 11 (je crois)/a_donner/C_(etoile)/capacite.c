
#include "capacite.h"


double capacite_max(graph_t* g, int x, int y) {
    //TODO
}


edge_t *prim(graph_t *g){
    //TODO
}


edge_t* arbre_max(graph_t* g){
    //TODO
}

double cmax(graph_t* g, int x, int y){
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
