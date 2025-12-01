#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>

bool chercheRK(const char* fichier, const char* mot) {
    FILE* f = fopen(fichier, "r");

    int h = 0;
    int h_correct = 0;

    char chaine[1000] = "";

    int nb_ligne = 1;

    while (fscanf(f, "%[^\n] ", chaine) != EOF) {
        // On traite la recherche ligne par ligne
        
        int len = strlen(mot);

        if ((int) strlen(chaine) < len) {
            continue;
        }

        // On calcule le hash de départ
        for (int i = 0 ; i < len; i++) {
            h += chaine[i];

            h_correct += mot[i];
        }

        //printf("%d\n", nb_ligne);
        printf("%d\n", nb_ligne);
        nb_ligne++;

        // On parcourt la suite du tableau
        for (int i = len; i < (int) strlen(chaine); i++) {
            h += chaine[i];
            h -= chaine[i-len];

            //printf("%d\n", h);

            if (h == h_correct) {
                // Les 2 hash sont égaux, on fait une comparaison brut
                bool OK = true;
                for (int j = 0; OK && j < len; j++) {
                    if (chaine[i+j-len+1] != mot[j]) {
                        OK = false;
                    }
                }
                if (OK) {
                    fclose(f);
                    return true;
                }
            }
        }
    }

    fclose(f);

    return false;
}

int main(int argc, char* argv[]) {
    assert(argc == 3);


    printf("%d\n", chercheRK(argv[1], argv[2]));

    return 0;
}