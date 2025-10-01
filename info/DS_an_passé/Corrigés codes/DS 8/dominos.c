#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dominos.h"
#include "corrigeDominos.h"

/* pour les tests, on va avoir besoin d'une fonction d'affichage ! Ce n'est jamais perdu d'en écrire une ! */

void afficheDomino(Domino d){
    printf("(%d,%d)", d.x, d.y);
    return;
}

void affiche_chaine(element* l){
    element* maillon_actuel = l;
    while (maillon_actuel != NULL) {
        afficheDomino(maillon_actuel->d);
        maillon_actuel = maillon_actuel->suivant;
    }
}


element* ajoutElement(element* l, Domino d) {
    if (l == NULL) {
        element* elt = (element*) malloc(sizeof(element));
        elt->d = d;
        elt->suivant = NULL;
        return elt;
    } else {
        l->suivant = ajoutElement(l->suivant, d); // NB : il est indispensable de mettre à jour suivant (cas où l->suivant == NULL)
        return l;
    }
}

element* retireElement(element* l, Domino d) {
    if (l==NULL) return NULL;
    else {
        Domino tete = l->d;
        if (tete.x == d.x && tete.y == d.y) { //domino à supprimer
            element* res = l->suivant;
            free(l);
            return res;
        } else { //on retire récursivement
            l->suivant = retireElement(l->suivant, d);
            return l;
        }
    }
}

bool rechercheElement(element* l, Domino d) {
    return l != NULL && (((l->d).x == d.x && (l->d).y == d.y) || rechercheElement(l->suivant, d));
}

bool chercheElementLisible(element* l, Domino d) {
    if (l == NULL) return false;
    else {
        Domino tete = l->d;
        if (tete.x == d.x && tete.y == d.y) return true;
        else return chercheElementLisible(l->suivant, d);
    }
}


/* Problème 1 */

bool possible(Domino Di, Domino Dj) {
    return Di.y == Dj.x;
}

bool possibleAvecRotation(Domino Di, Domino* Dj) {
    if (Di.y == Dj->y) { //attention à ne pas confondre le . et la ->
        int t = Dj->y; //rotation du Domino
        Dj->y = Dj->x;
        Dj->x = t;
        return true;
    }
    else {return false;}
}



//On recode ajoutElement pour que l'ajout se fasse en tête, la complexité sera bien meilleure (et la fonction plus facile à écrire) !
element* ajoutElementTete(element* l, Domino d) {
    element* elt = (element*) malloc(sizeof(element));
    elt->d = d;
    elt->suivant = l;
    return elt;
}


/* On propose ici un code très proche du pseudo-code donné.
    Pour pouvoir ajouter un domino dans le sac ou la chaine partielle, il faut pouvoir les MODIFIER, donc on donne des pointeurs vers le sac (element*)* et vers la chaîne partielle.
    Sinon, le sac et la chaîne seront passées par valeur, on donnera bien une nouvelle valeur aux appels enfants,
    mais les appels parents ne verront pas les modifs de leurs enfants sur sac/chaîne. */
bool chercheChaineComplete(element** sac, element** TeteChainePartielle) {
    //Note : on ajoute les dominos en tête et non en bout, pour une bien meilleure complexité (à rotations près, ça ne change rien)
    if (*sac == NULL) {
        printf("Chaîne trouvée  : "); 
        affiche_chaine(*TeteChainePartielle); 
        printf("\n");
        return true;
    } else {
        element* maillon_actuel = *sac;
        while (maillon_actuel != NULL) {
            Domino d = maillon_actuel->d;
            maillon_actuel = maillon_actuel->suivant; //pour la prochaine itération de la boucle, avant de perdre le maillon actuel (retireElement va le libérer)
            *sac = retireElement(*sac, d); //Remarque : mauvaise complexité ici (mais on fait avec ce qu'on a), il serait plus efficace de retirer le maillon suivant à chaque fois pour aller bien plus vite, au lieu de retirer le maillon actuel (on sait où il est). Mais il faudrait traiter la tête à part.
            if (*TeteChainePartielle == NULL) {
                *TeteChainePartielle = ajoutElementTete(*TeteChainePartielle, d);
                if (chercheChaineComplete(sac, TeteChainePartielle)) return true;
                *TeteChainePartielle = retireElement(*TeteChainePartielle, d); //ATTENTION : nécessaire pour les fuites de mémoire (et pour retourner dans la chaine précédente)
            }
            else if (possible((*TeteChainePartielle)->d, d)) {
                *TeteChainePartielle = ajoutElementTete(*TeteChainePartielle, d);
                if (chercheChaineComplete(sac, TeteChainePartielle)) return true; 
                *TeteChainePartielle = retireElement(*TeteChainePartielle, d);
            }
            else if (possibleAvecRotation((*TeteChainePartielle)->d, &d)) {
                *TeteChainePartielle = ajoutElementTete(*TeteChainePartielle, d); //ici d a été modifié !
                if (chercheChaineComplete(sac, TeteChainePartielle)) return true; 
                *TeteChainePartielle = retireElement(*TeteChainePartielle, d);
            }
            *sac = ajoutElementTete(*sac, d);

        }
        return false;
    }
}

element* renverse(element* chaine){
    element* renverse = NULL;
    element* maillon_actuel = chaine;
    while (maillon_actuel != NULL) {
        element* suiv = maillon_actuel->suivant;
        maillon_actuel->suivant = renverse;
        renverse = maillon_actuel;
        maillon_actuel = suiv;
    }
    return renverse;
}

element* chaineComplete(element* sac) {
    element* chainePartielle = NULL;
    chercheChaineComplete(&sac, &chainePartielle); //on ignore le booléen renvoyé, on s'en moque
    return renverse(chainePartielle);
}


void libere_chaine(element* l){
    if (l != NULL) {
        libere_chaine(l->suivant);
        free(l);
    } 
}

int main(int argc, char* argv[]){
    Domino d1 = { .x = 5, .y = 4};
    Domino d2 = { .x = 4, .y = 0};
    Domino d3 = { .x = 0, .y = 1};
    Domino d4 = { .x = 2, .y = 3};
    Domino d5 = { .x = 3, .y = 5};

    element* sac = NULL;
    
    
    sac = ajoutElementTete(sac, d1);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");

    sac = ajoutElement(sac, d2);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");

    sac = ajoutElement(sac, d3);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");

    sac = ajoutElement(sac, d4);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");

    sac = ajoutElement(sac, d5);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");
    

    /*
    sac = ajoutElementTete(sac, d1);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");

    sac = ajoutElementTete(sac, d2);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");
    */

    /*
    sac = retireElement(sac,d2);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");
    sac = retireElement(sac,d5);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");
    sac = retireElement(sac,d4);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");
    sac = retireElement(sac,d3);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");
    sac = retireElement(sac,d1);
    printf("Contenu du sac : "); affiche_chaine(sac); printf("\n");
    */

    element* chaine = chaineComplete(sac);
    printf("Test de construction de chaîne complète sur l'exemple donné dans l'énoncé :\n");
    affiche_chaine(chaine); printf("\n");

    libere_chaine(sac);

    return EXIT_SUCCESS;
}
