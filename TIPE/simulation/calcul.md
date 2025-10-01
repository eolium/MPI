# Idées pour le calcul

- Pour ne pas surcharger la mémoire, ne pas stocker les mesures intermédiaires, ne stocker que les mesures locales (maj régulières)
- Stocker une liste des noeuds indirectes pour rapidement renvoyer un UnknownNodeError si le noeud n'est pas accessible

-> Pour effectuer un calcul :
    - Si la requête vient de nous, on génère un UID

    - On envoie à chaque noeud connexe une requête de calcule avec :
        - [uint] UID + profondeur
        - [uint] s_s
        - [bool] TTL/profondeur max
        - [bool] marquage multiple autorisé


-> Pour répondre à une requête :
    - Si on a déjà récupéré une mesure, on la renvoie

    - Sinon {
        - On envoie une requête de calcul
    }



-> Pour effectuer une mesure :
    - 