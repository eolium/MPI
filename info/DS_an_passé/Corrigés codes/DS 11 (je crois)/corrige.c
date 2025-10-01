#include "corrige.h"


int nb_edges_cor(graph_t* g){
    int p = 0;
    for (int x = 0; x < g->n; x+=1) {
        p += g->degrees[x];
    }
    return p / 2;
}

double** matrice_initiale_cor(graph_t* g){
    //Création de la matrice qu'on va remplir ensuite
    double** m0 = malloc(sizeof(double*) * g->n);
    for (int i = 0 ; i < g->n ; i +=1 ) {
        m0[i] = malloc(sizeof(double) * g->n);
        //On initialise avec -inf ou +inf sans prendre en compte les arêtes pour l'instant
        for (int j = 0; j < g->n ; j+=1 ) {
            if (i == j) m0[i][j] = INFINITY;
            else m0[i][j] = -INFINITY;
        }
    }

    //Remplissage de la matrice avec les arêtes du graphe.
    for (int u = 0 ; u < g->n ; u +=1 ) {
        for (int iv = 0; iv < g->degrees[u] ; iv+=1 ) {
            edge_t ar = g->adj[u][iv];
            m0[u][ar.y] = ar.weight;
        }
    }

    return m0;
}

void free_matrice_cor(double** m, int n){
    for (int u = 0 ; u < n ; u +=1 ) {
        free(m[u]);
    }
    free(m);
}

double** copy_matrice_cor(double** m, int n){
    double** m_copie = malloc(sizeof(double*) * n);
    for (int i = 0 ; i < n ; i +=1 ) {
        m_copie[i] = malloc(sizeof(double) * n);
        for (int j = 0; j < n ; j+=1 ) {
            m_copie[i][j] = m[i][j];
        }
    }
    return m_copie;
}

double max_cor(double x, double y){
    if (x >= y) return x;
    else return y; 
}

double min_cor(double x, double y){
    if (x <= y) return x;
    else return y; 
}

//On pourrait gagner du temps en travaillant tout du long dans la même matrice, comme avec Floyd-Warshall, 
//mais on n'a pas prouvé la propriété associée.
double capacite_max_cor(graph_t* g, int x, int y) {
    double** m = matrice_initiale_cor(g);
    for (int k = 0; k < g->n ; k +=1 ) {

        //création de la nouvelle matrice
        double** m_copie = copy_matrice_cor(m, g->n);

        //remplissage de la nouvelle matrice dans m grâce à sa copie
        for (int i = 0; i < g->n ; i+=1 ) {
            for (int j = 0; j < g->n; j+=1) {
                m[i][j] = max_cor(m_copie[i][j], min_cor(m_copie[i][k], m_copie[k][j]));
            }
        }

        //libération de l'ancienne matrice
        free_matrice_cor(m_copie, g->n);
    }
    return m[x][y];
}




void add_edges_cor(graph_t *g, heap_t *heap, bool *covered, int x){
    int degree = g->degrees[x];
    for (int i = 0; i < degree; i++){
        edge_t e = g->adj[x][i];
        if (!covered[e.y]) heap_push(heap, e);
    }
}

edge_t *prim_cor(graph_t *g){
    edge_t *selected = malloc((g->n - 1) * sizeof(edge_t));
    int nb_selected = 0;

    bool *covered = malloc(g->n * sizeof(bool));
    for (int i = 0; i < g->n; i++) covered[i] = false;

    heap_t *heap = heap_create(g->p);

    covered[0] = true;
    add_edges_cor(g, heap, covered, 0);

    while (!heap_is_empty(heap)){
        edge_t e = heap_extract_min(heap);
        if (covered[e.y]) continue;
        covered[e.y] = true;
        selected[nb_selected] = e;
        nb_selected++;
        add_edges_cor(g, heap, covered, e.y);
    }

    free(covered);
    heap_free(heap);
    return selected;
}



/* ----- Questions étoilées  ----- */
/* Ci dessous, les questions qui n'étaient à traiter qu'en fin de sujet étoilé, difficilement atteignables. */



// --- Question arbre_max : ---

graph_t* graphe_oppose_cor(graph_t* g){
    graph_t* gm = malloc(sizeof(graph_t));
    gm->n = g->n;
    gm->p = g->p;
    gm->degrees = malloc(g->n * sizeof(int));
    memcpy(gm->degrees, g->degrees, g->n*sizeof(int));
    gm->adj = malloc(g->n * sizeof(edge_t*));
    for (int i = 0; i < g->n; i++){
        gm->adj[i] = malloc(g->degrees[i] * sizeof(edge_t));
        memcpy(gm->adj[i], g->adj[i], g->degrees[i]*sizeof(edge_t));
        for (int j = 0; j < g->degrees[i]; j++) gm->adj[i][j].weight = - gm->adj[i][j].weight;
    }
    return gm;
}

edge_t* arbre_max_cor(graph_t* g){
    graph_t* gm = graphe_oppose_cor(g);
    edge_t* abr = prim_cor(gm);
    graph_free(gm);
    for (int i = 0; i < g->n - 1; i++){
        abr[i].weight = -abr[i].weight;
    }
    return abr;
}


// --- Question capacité max : ---

// construit l'arbre sous la forme d'un graphe pour lancer plus facilement un parcours dessus
graph_t* graph_from_edges_cor(edge_t* ar, int len, int n){
    graph_t* g = malloc(sizeof(graph_t));
    g->n = n;
    g->p = len;
    g->degrees = calloc(n, sizeof(int));
    //premier passage pour connaître les degrés et les dimensions de adj
    for (int i = 0; i < len; i++){
        edge_t a = ar[i];
        g->degrees[a.x]++;
        g->degrees[a.y]++;
    }

    //création de adj aux bonnes dimentions
    g->adj = malloc(n * sizeof(edge_t*));
    for (int i = 0; i < n; i++){
        g->adj[i] = malloc(g->degrees[i] * sizeof(edge_t));
    }

    //remplissage des arêtes données dans adj
    int* indices = calloc(n, sizeof(int)); //indices actuels dans chaque tableau d'adjacence
    for (int i = 0; i < len; i++){ //len = n-1 pour un arbre
        edge_t a = ar[i];
        g->adj[a.x][indices[a.x]] = a;
        edge_t a_inv = {.x = a.y, .y = a.x, .weight = a.weight};
        g->adj[a.y][indices[a.y]] = a_inv;
        indices[a.x]++;
        indices[a.y]++; 
    }
    return g;
}


//Parcours de graphe (fonction auxiliaire de parcours récursif)
void parcours_rec_cor(graph_t* g, int u, bool* deja_vu, edge_t* pred){
    deja_vu[u] = true;
    for (int iv = 0; iv < g->degrees[u]; iv++){
        edge_t arc = g->adj[u][iv];
        if (!deja_vu[arc.y]) {
            pred[arc.y] = arc;
            parcours_rec_cor(g, arc.y, deja_vu, pred);
        }
    }
}

//Parcours de graphe g depuis le sommet s
edge_t* arbre_parcours_cor(graph_t* g, int s){
    bool* deja_vu = malloc(g->n * sizeof(bool));
    for (int i = 0; i < g->n ; i++) deja_vu[i] = false;
    edge_t* pred = malloc(g->n * sizeof(edge_t));
    for (int i = 0; i < g->n ; i++) {
        edge_t a_vide = {.x = -1, .y = -1, .weight = -1};
        pred[i] = a_vide ;
    }
    edge_t a_racine = {.x = s, .y = s, .weight = 0};
    pred[s] = a_racine;
    parcours_rec_cor(g, s, deja_vu, pred);
    free(deja_vu);
    return pred;
}


double cmax_cor(graph_t* g, int x, int y){
    edge_t* ar = arbre_max_cor(g);
    graph_t* abr = graph_from_edges_cor(ar, g->n - 1, g->n);
    edge_t* pred = arbre_parcours_cor(abr, x);

    //le chemin entre x et y se lit en remontant les prédécesseurs depuis y.
    //Ici on ne s'intéresse qu'à la capacité max, et on la calcule directement.
    if (pred[y].x == -1) { //pas de chemin entre x et y
        return -INFINITY;
    }
    int sommet_actuel = y;
    double troncon_min = INFINITY;
    while (sommet_actuel != pred[sommet_actuel].x){
        edge_t arc = pred[sommet_actuel];
        if (arc.weight < troncon_min) troncon_min = arc.weight;
        sommet_actuel = arc.x;
    }
    return troncon_min;
}
