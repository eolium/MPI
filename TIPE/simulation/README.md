# Simulation du problème

On se place dans un graphe à N sommets, dans lequel on veut calculer l'angle et la distance de s_0 à S.
Pour simplifier, et puisque cela revient au même (modulo un Al-Kashi), on appelerra mesure d'angle et distance une "mesure".

Idée :
    - on construit un graphe des plus proches sommets, en supposant qu'ils répondront plus rapidement (sinon peut importe l'ordre)
    - On lance récursivement un calcul de l'angle dans tout le graphe, avec un marquage récursif des sommets
        - Si la requête n'a pas déjà été reçu {
            - On lance la requête M avec un UID.
            - Les sommets renvoient une mesure
            - On réalise la moyenne pondérée de ces mesures (par taux d'erreur depuis le début + écart itératif à la moyenne)
        }


- Avantages :
    - Pas besoin de stocker tout le graphe, il faut juste numéroter les sommets

    - Plus un sommet renvoie des erreurs, moins on le "croie"
    - Plus un sommet est écarté de la moyenne, moins on le "croie"


Pour mesurer :
    - Si on reçoit une requête DIST, on la traite en priorité.

    - Si on reçoit une requête de mesure, on la traite.

    - Sinon, on maj les arêtes les plus vieilles.



Algo :

setup (toutes les 20 secondes) {
    - On construit un graphe des plus proches sommets
        - Requête BROADCAST, on traite les réponses les plus rapides

    - On mesure les distances et les angles par requête DIST pour chaque sommets
}

loop {
    - Si requête DIST -> ON TRAITE

    - Si requête CALC -> on traite récursivement et on update les infos en mémoire
        -> Le graphe peut grossir ? (Oui, en comptant les dernière dates de requête)

    - Si requête broadcast -> On traite avec une proba {p} (qui varie en fonction de la densité du graphe)
        -> Broadcast itératif : d'abord avec p petit, pour prédire la taille du graphe, puis p en fonction de la taille du graphe

    (On/Off) {
        - On allume l'écran
    }
    - On affiche un menu avec les contacts

    {Liaison} {
        - On lie les 2 appareils, on demande un nom de contact 
    }

    {Find(x)} {
        - Toutes les 3 secondes {
            - On calcule récursivement une mesure, qu'on affiche.
                -> forcer la récursivité permet d'avoir une moyenne des mesures de tous les noeuds -> moyenne
        }
    }

    - Toutes les 3 secondes {
        - On maj les infos
    }
}

Requête DIST(s_e, s_s, UID, TTL, last_dist) {
    - si TTL=0 la requête meurt
    - si marquage[UID]=true la requête meurt

    - si s_s != NOEUD {
        - si dist_connu < last_dist {
            - on transmet la requête
        } sinon {
            - on renvoie le résultat
        }
    }

    Idées :
        - si on se rapproche du noeud on transmet la requête, sinon on la tue
}
