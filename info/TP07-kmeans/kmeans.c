#include "kmeans.h"

int square(int x) {
  return x*x;
}

/** Renvoie le CARRÉ de la distance entre deux pixels */
int sqd(pixel p0, pixel p1) {
  int dr = square((int) p0.r - (int) p1.r);
  int dg = square((int) p0.g - (int) p1.g);
  int db = square((int) p0.b - (int) p1.b);
  int da = square((int) p0.a - (int) p1.a);

  return dr + dg + db + da;
}

/** Modifie l'isobarycentre de tous les clusters. */
void maj_isobar(const pixel* img, const int* cluster, int N,
                    const int* card, pixel* isobar, int k) {

  // /!\ On fera attention à éviter les overflows. Il faut faire les calculs
  // (sommes de rouge, bleu, etc) dans des int, et retransformer en char le résultat final.

  /*
    On fait un tableau qui contient la somme de chaque cluster
    4i : r
    4i+1 : g
    4i+2 : b
    4i+3 : a
  */
  int* somme = malloc(k * 4 * sizeof(int));
  
  for (int i = 0; i < 4*k; i++) {
    somme[i] = 0;
  }

  for (int i = 0; i < N; i++) {
    int cl = cluster[i];

    somme[4 * cl + 0] += (int) img[i].r;
    somme[4 * cl + 1] += (int) img[i].g;
    somme[4 * cl + 2] += (int) img[i].b;
    somme[4 * cl + 3] += (int) img[i].a;
  }

  for (int i = 0; i < k; i++) {
    if (card[i] > 0) {
      isobar[i].r = (uint8_t) (somme[4 * i + 0] / card[i]);
      isobar[i].g = (uint8_t) (somme[4 * i + 1] / card[i]);
      isobar[i].b = (uint8_t) (somme[4 * i + 2] / card[i]);
      isobar[i].a = (uint8_t) (somme[4 * i + 3] / card[i]);
    }
  }

  free(somme);
}


/** Renvoie le numéro du cluster associé à un pixel. */
int trouve_cluster(pixel p, const pixel* isobar, int k) {
  int choix = 0;
  int d_choix = sqd(p, isobar[0]);

  for (int i = 1; i < k; i++) {
    int d = sqd(p, isobar[i]);

    if (d < d_choix) {
      choix = i;
      d_choix = d;
    }
  }

  return choix;
}


/** Remplit le tableau `isobar` avec k centres initialement choisis au hasard parmi les pixels (deux à deux distincts). */
void initialise_centres(int k, const pixel* img, int N, pixel* isobar) {
  /* On fera un mélange de Fisher-Yates des indices des pixels {0, ..., N-1}, et on sélectionnera les pixels des k premiers indices du mélange dans isobar. */


  // On recopie le tableau
  int* tab = malloc(N * sizeof(pixel));

  for (int i = 0; i < N; i++) {
    tab[i] = i;
  }


  // On mélange le tableau copié
  for (int i = N-1; i >= 1; i--) {
    int j = rand() % i;

    int temp = tab[j];
    tab[j] = tab[i];
    tab[i] = temp;
  }

  // On prend les k premiers pixels
  for (int i = 0; i < k; i++) {
    isobar[i] = img[tab[i]];
  }

  free(tab);
}



/** Algorithme des k moyennes.*/
int kmeans(int k, const pixel* img, int N, int* cluster, pixel* isobar) {

  /* Initialisation */

  initialise_centres(k, img, N, isobar);

  int* card = malloc(k * sizeof(int));
  
  /* Itérations de l'algorithme des k moyennes */

  /*
  On réinitialise les variables nécessaires
  */

  int iterations = 0;
  int nb_modifs = 1;

  while (nb_modifs) {
    nb_modifs = 0;

    for (int i = 0; i < k; i++) {
      card[i] = 0;
    }

    for (int i = 0; i < N; i++) {
      int cl = trouve_cluster(img[i], isobar, k);

      if (cluster[i] != cl) {
        cluster[i] = cl;
        nb_modifs++;
      }

      card[cl]++;
    }

    maj_isobar(img, cluster, N, card, isobar, k);

    iterations++;

    printf("\r%5d" " itérations effectuées, %5d modifications effectuées.", iterations, nb_modifs);
    fflush(stdout);
  }

  /* Fin */

  free(card);

  return iterations;
}
/* ASTUCE : insérer cette ligne dans votre code si vous souhaitez voir joliment la progression de votre clustering sur la grosse image : 
printf("\r%5d" " itérations effectuées.", nb_iter);
(avec nb_iter le nombre actuel d'itéations effectuées).
*/


int main(int argc, char* argv[]) {

  if (argc < 2) {
    fprintf(stderr, "Usage : %s [add-alpha | compress]\n", argv[0]);
    return EXIT_FAILURE;
  }

  srand(time(NULL)); /* Initialise la génération aléatoire d'entiers.
                        L'initialisation dépend de l'heure (en s) depuis le 1er janvier 1970, 
                        donc change à chaque seconde.
                      */

  
  /* Add-alpha : crée une copie de l'image donnée. On
     garantit que cette copie dispose d'un canal alpha.. */
  if (strcmp(argv[1], "add-alpha") == 0) {

    char* input_file = argv[2];
    char* output_file = argv[3];
    if (strcmp(input_file, output_file) == 0) {
      fprintf(stderr, "Par mesure de sécurité, les fichiers d'entrée "
                      "et de sortie doivent être distincts.\n");
      return EXIT_FAILURE;
    }

    int nb_row;
    int nb_col;
    printf("Lecture de %s...\n", input_file);
    fflush(stdout);
    pixel* img = decode_png(input_file, &nb_row, &nb_col);
    printf("Écriture de %s...\n", output_file);
    fflush(stdout);
    encode_png(output_file, img, nb_row, nb_col);
    printf("Fin.\n");
    free(img);

    return EXIT_SUCCESS;
  }

  else if (strcmp(argv[1], "compress") == 0) {

    // Lecture des entrées
    if (argc != 5) {
      fprintf(stderr, "Usage : %s compress input_file output_file nb_de_couleurs.\n", argv[0]);
      return EXIT_FAILURE;
    }
    
    char* input_file = argv[2];
    char* output_file = argv[3];
    if (strcmp(input_file, output_file) == 0) {
      fprintf(stderr, "Par mesure de sécurité, les fichiers d'entrée "
                      "et de sortie doivent être distincts.\n");
      return EXIT_FAILURE;
    }

    int nb_row = 0;
    int nb_col = 0;
    printf("Lecture de %s...\n", input_file);
    fflush(stdout);
    pixel* img = decode_png(input_file, &nb_row, &nb_col);
    int N = nb_row * nb_col;
    
    int k = atoi(argv[4]);
    if (k < 0 || k > N) {
      fprintf(stderr, "Un nombre incohérent de couleurs a été demandé. "
                      "Il a été demandé %d couleurs; l'image %s à compresser "
                      "a %d pixels.\n", k, input_file, N);
      free(img);
      return EXIT_FAILURE;
    }
    
    // Utilisation de kmeans
    int* cluster = (int*) malloc(N*sizeof(int));
    pixel* isobar = (pixel*) malloc(k*sizeof(pixel));
    printf("Début de l'algorithme des k-moyennes...\n");
    fflush(stdout);
    int nb_iter = kmeans(k, img, N, cluster, isobar);
    if(nb_iter < 0) {
      fprintf(stderr, "L'algorithme des k-moyennes n'a pas encore  "
                      "été implémenté.\n");
      free(img);
      free(isobar);
      free(cluster);
      return EXIT_FAILURE;
    }
    printf("\nL'algorithme des k-moyennes a convergé en %d itérations.\n", nb_iter);

    // Grand final !
    for (int i = 0; i < N; i+=1) {
      img[i] = isobar[cluster[i]];
    }
    printf("Écriture de %s...\n", output_file);
    fflush(stdout);
    encode_png(output_file, img, nb_row, nb_col);
    printf("Fin.\n");

    free(img);
    free(isobar);
    free(cluster);
    return EXIT_SUCCESS;

  }

  else {
    fprintf(stderr, "Usage : %s [add-alpha | remove-alpha | compress]\n", argv[0]);
    return EXIT_FAILURE;
  }
}