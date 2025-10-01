type sommet = int
type graphe = sommet list array

type arete = {x : sommet; y : sommet}

type couplage = arete list
type graphe_biparti = { g : graphe; partition : bool array }

(* graphe biparti du cours : *)
let gb = 
  let g = [|
    [8; 9];    (* 0  *)
    [9; 10];    (* 1  *)
    [8; 11]; (* 2  *)
    [11];       (* 3  *)
    [7; 8];       (* 4  *)
    [10; 12];    (* 5  *)
    [12];       (* 6  *)
    [4];        (* 7  *)
    [0; 2; 4];   (* 8  *)
    [0; 1];   (* 9  *)
    [1; 5];   (* 10 *)
    [2; 3];   (* 11 *)
    [5; 6]    (* 12 *)
  |]
  in { g = g; partition = [| true; true; true; true; true; true; true; false; false; false; false; false; false |] }

(* graphe à 6 sommets pour lequel le plus grand couplage possible ne couvre que 4 sommets *)
let gb2 =
  let g = [|
   [1];  (* 0 *) 
   [0; 2; 4];  (* 1 *) 
   [1];  (* 2 *) 
   [4];  (* 3 *) 
   [1; 3; 5];  (* 4 *) 
   [4]  (* 5 *) 
  |]
  in { g = g; partition = [| true; false; true; false; true; false |]}


let gb_non_connexe =
  let g = [|
    [8; 9];    (* 0  *)
    [9; 10];    (* 1  *)
    [8; 11]; (* 2  *)
    [11];       (* 3  *)
    [7; 8];       (* 4  *)
    [10; 12];    (* 5  *)
    [12];       (* 6  *)
    [4];        (* 7  *)
    [0; 2; 4];   (* 8  *)
    [0; 1];   (* 9  *)
    [1; 5];   (* 10 *)
    [2; 3];   (* 11 *)
    [5; 6];    (* 12 *)

    [14];  (* 13 *) 
    [13; 15; 17];  (* 14 *) 
    [14];  (* 15 *) 
    [17];  (* 16 *) 
    [14; 6; 18];  (* 17 *) 
    [17]  (* 18 *) 
  |]
  in { g = g; partition = [| true; true; true; true; true; true; true; false; false; false; false; false; false; true; false; true; false; true; false  |] }





(* On ne cherchera pas toujours à faire des fonctions de complexité optimale dans ce chapitre. *)

let rec est_dans_couplage_cor ar c = match c with
  [] -> false
  | h :: t -> if h.x = ar.x && h.y = ar.y || h.x = ar.y && h.y = ar.x then true
  else est_dans_couplage_cor ar t


let difference_symetrique_cor c c' = (* renvoie un nouveau couplage : c delta c'. *)
  List.filter (fun ar -> not (est_dans_couplage_cor ar c')) c
  @ List.filter (fun ar -> not (est_dans_couplage_cor ar c)) c'



let rec est_couvert_cor s c =
  match c with
  [] -> false
  | h :: t -> (s = h.x || s = h.y) || est_couvert_cor s t 


let graphe_de_couplage_cor gb c = (* renvoie le graphe du couplage avec s et t en plus, sommet d'indice n et n+1. *)
  let n = Array.length gb.g in
  let s = n in
  let t = n + 1 in
  let gc = Array.make (n+2) [] in

  (* arcs de s vers les sommets libres de X et des sommets libres de T vers t : *)
  for u = 0 to (n-1) do
    if not (est_couvert_cor u c) then begin (* sommet libre *)
      if gb.partition.(u) = true then (* sommet libre dans X, à connecter à s = n *)
        gc.(s) <- u :: gc.(s)
      else (* sommet libre dans Y, à connecter à t = n+1 *)
        gc.(u) <- t :: gc.(u)
    end
  done;

  (* arcs x -> y du graphe de départ *)
  for u = 0 to (n-1) do
    if gb.partition.(u) then begin (* on ne regarde que les arcs X -> Y, pas Y -> X *)
      let traite_voisin v = (* arete de u \in X à v \in Y *)
        let ar = {x = u; y = v} in    
        if est_dans_couplage_cor ar c then (* arete dans C *)
          gc.(v) <- u :: gc.(v)
        else (* arete dans A \ C *)
          gc.(u) <- v :: gc.(u)
      in List.iter traite_voisin gb.g.(u)
    end
  done;
  gc



let arbre_parcours_cor g s = (* renvoie le tableau des prédécesseurs dans un parcours de g depuis s (quelconque) *)
  let n = Array.length g in
  let deja_vu = Array.make n false in
  let pred = Array.make n (-1) in
  pred.(s) <- s;
  let rec parcours u =
    deja_vu.(u) <- true;
    let traite_voisin v =
      if not deja_vu.(v) then begin
        pred.(v) <- u;
        parcours v
      end
    in List.iter traite_voisin g.(u)
  in
  parcours s;
  pred


let chemin_cor g s t = (* renvoie un chemin de s à t dans g, sous la forme d'une liste d'arête *)
  let pred = arbre_parcours_cor g s in
  if pred.(t) = -1 then [] (* t n'est pas accessible depuis s *)
  else if t = s then [] (* on n'a besoin d'aucune arête pour aller de s à s *)
  else begin
    let rec construit_chemin u c =
      let parent = pred.(u) in
      if parent = u then c (* cas de base, on a atteint la racine s de l'arbre, plus d'arête à parcourir *)
      else
        let ar = {x = parent; y = u} in
        construit_chemin parent (ar :: c)
    in construit_chemin t []
  end


(* version récursive et fonctionnelle *)
let couplage_maximum_biparti_cor gb =
  let n = Array.length gb.g in
  let rec augmente_couplage c =
    let gc = graphe_de_couplage_cor gb c in
    let p = chemin_cor gc n (n+1) in (* p privé de s et t est un chemin augmentant si p existe *)
    if p = [] then c (* cas de base : on renvoie le couplage précédent qui était de cardinalité maximale *)
    else begin
      let p_sans_t = List.rev (List.tl (List.rev p)) in
      let p_sans_t_ni_s = (List.tl p_sans_t) in
      augmente_couplage (difference_symetrique_cor c p_sans_t_ni_s)
    end
  in augmente_couplage []
