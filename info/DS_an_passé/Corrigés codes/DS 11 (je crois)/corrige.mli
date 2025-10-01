type sommet = int
type graphe = sommet list array

type arete = {x : sommet; y : sommet}

type couplage = arete list
type graphe_biparti = { g : graphe; partition : bool array }



val est_dans_couplage_cor : arete -> arete list -> bool

val difference_symetrique_cor : arete list -> arete list -> arete list

val est_couvert_cor : sommet -> arete list -> bool

val graphe_de_couplage_cor : graphe_biparti -> arete list -> graphe

val arbre_parcours_cor : graphe -> sommet -> sommet array

val chemin_cor : graphe -> sommet -> sommet -> arete list

val couplage_maximum_biparti_cor : graphe_biparti -> arete list
