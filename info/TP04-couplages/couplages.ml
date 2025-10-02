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
  




(* ----- Fonctions ----- *)

let rec est_dans_couplage (ar: arete) (c: couplage) = 
  let a = ar.x in
  let b = ar.y in
  match c with
  | [] -> false
  | h::q ->
    let x = h.x in
    let y = h.y in
    (x = a && y = b) || (x = b && y = a) || est_dans_couplage ar q



let difference_symetrique (c1: couplage) (c2: couplage) = 
  let out = ref c1 in
  List.iter (fun ar -> if not (est_dans_couplage ar c2) then out := ar::!out) c2;
  !out

(* renvoie vrai si s est une extrémité d'une des arêtes du couplage, faux sinon *)
let rec est_couvert (s: sommet) (c: arete list) =
  match c with
  | [] -> false
  | ar::q ->
    let x = ar.x in
    let y = ar.y in
    s = x || s = y || est_couvert s q



(** renvoie le graphe (orienté) du couplage gc, 
  les sommets s et t sont les sommets d'indice n et n+1. *)
let graphe_de_couplage (gb: graphe_biparti) (c: couplage) = 
  let len = Array.length gb.g in
  let out = Array.make len [] in

  for i = 0 to len -1 do
    List.iter (fun v ->
      if est_couvert v c then
        out.(i) <- v::out.(i)
    ) gb.g.(i)
  done;

  (out: graphe)


(* renvoie le tableau des prédécesseurs dans un parcours (quelconque) de g depuis s *)
(*
let arbre_parcours (g: graphe) (s: sommet) = 
  let len = Array.length g in
  let pred = Array.make len (-1) in

  let file_parcours = Queue.create () in
  Queue.add s file_parcours;

  while Queue.length file_parcours > 0 do

  done;
  
  pred
*)
let arbre_parcours g s =
  failwith "TODO"


(* Construis un chemin de s à t dans g, s'il existe, sous la forme d'une liste d'arêtes.
  S'il n'en existe pas, on renvoie le chemin vide []. *)
let chemin g s t =
  failwith "TODO"



let couplage_maximum_biparti gb =
  failwith "TODO"

