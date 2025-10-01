open CorrigeRandonneurs

type chemin_caillou = int array

let caillou_vide chemin =
  let position = ref (-1) in
  let n = Array.length chemin in
  for i = 0 to n-1 do
      if chemin.(i) = 0 then
          position := i
  done;
  !position

let echange_en_place tab i j =
  let tmp = tab.(i) in
  tab.(i) <- tab.(j);
  tab.(j) <- tmp

let echange chemin i j =
  let chemin2 = Array.copy chemin in
  echange_en_place chemin2 i j;
  chemin2

let randonneurG_avance chemin =
  let i = caillou_vide chemin in
  i >= 1 && chemin.(i-1) = 1
  
let randonneurG_saute chemin =
  let i = caillou_vide chemin in
  i >= 2 && chemin.(i-2) = 1

let randonneurD_avance chemin =
  let i = caillou_vide chemin in
  i < Array.length chemin - 1 && chemin.(i+1) = 2
  
let randonneurD_saute chemin =
  let i = caillou_vide chemin in
  i < Array.length chemin - 2 && chemin.(i+2) = 2

let mouvement_chemin chemin =
  let mouvements = ref [] in
  let caillou = caillou_vide chemin in
  if randonneurG_avance chemin then
      mouvements := (echange chemin caillou (caillou-1)) :: !mouvements;
  if randonneurG_saute chemin then
      mouvements := (echange chemin caillou (caillou-2)) :: !mouvements;
  if randonneurD_avance chemin then
      mouvements := (echange chemin caillou (caillou+1)) :: !mouvements;
  if randonneurD_saute chemin then
      mouvements := (echange chemin caillou (caillou+2)) :: !mouvements;
  !mouvements

let init n1 v1 n2 v2 =
  let chemin = Array.make (n1 + 1 + n2) v1 in
  chemin.(n1) <- 0;
  for i = n1 + 1 to n1 + n2 do chemin.(i) <- v2 done;
  chemin


(* ----------------------------------------- *)
(* On propose deux versions. La première fait plus que ce qui est demandé : on crée la liste de toutes les façons possibles de faire traverser la berge.
C'est une sorte de retour sur trace. 
On pourrait l'écrire différemment pour s'arrêter dès qu'on a trouvé un chemin (plus efficace). 
Ou faire un parcours en largeur (deuxième version)*)

(* Version "pseudo-backtrack", on construit tous les chemins possibles et on en renvoie un (c'est le plus simple à écrire) : *)
let passage nG nD =
  let c       = init nG 1 nD 2 in
  let c_final = init nD 2 nG 1 in
  let rec cherche c suite =
      match suite with
      | [] -> if c = c_final then [[c]] else []
      | c' :: suite ->
          let chemins = cherche c' (mouvement_chemin c') in
          List.map (List.cons c) chemins @ cherche c suite
  in
  (cherche c (mouvement_chemin c))

(* ------------------------------------- *)


(* Version parcours en largeur : *)
let accessible c c_final =
  let trouve = ref false in
  let a_traiter = Queue.create () in
  Queue.add c a_traiter;
  let pred = Hashtbl.create (Array.length c) in (* joue le rôle de a_traiter *)
  Hashtbl.add pred c c;
  while not (Queue.is_empty a_traiter) && not !trouve do
    let config = Queue.take a_traiter in
    let traite_voisin v =
      if Hashtbl.find_opt pred v = None then begin
        Queue.add v a_traiter;
        Hashtbl.add pred v config;
        if v = c_final then trouve := true
      end
    in List.iter traite_voisin (mouvement_chemin config)
  done;
  !trouve


(* Version parcours en largeur : *)
let passage_optimal c c_final =
  let a_traiter = Queue.create () in
  Queue.add c a_traiter;
  let pred = Hashtbl.create (Array.length c) in (* joue le rôle de a_traiter *)
  Hashtbl.add pred c c;
  while not (Queue.is_empty a_traiter) do
    let config = Queue.take a_traiter in
    (* ici on pourrait arrêter le parcours avec une exception si on a atteint c_final pour gagner du temps *)
    let traite_voisin v =
      if Hashtbl.find_opt pred v = None then begin
        Queue.add v a_traiter;
        Hashtbl.add pred v config
      end
    in List.iter traite_voisin (mouvement_chemin config)
  done;
  let chemin = ref [c_final] in
  let sommet_actuel = ref c_final in
  while (Hashtbl.find pred !sommet_actuel <> !sommet_actuel) do
    sommet_actuel := Hashtbl.find pred !sommet_actuel;
    chemin := !sommet_actuel :: !chemin
  done;
  !chemin


(* Version parcours en largeur : *)
let passage nG nD =
  let c       = init nG 1 nD 2 in
  let c_final = init nD 2 nG 1 in
  passage_optimal c c_final