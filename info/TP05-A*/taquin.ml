let n = 4

type etat = {
  grille : int array array;
  mutable i : int;
  mutable j : int;
  mutable h : int;
}

let affiche_etat etat =
  let taille = Array.length etat.grille in
  for i = 0 to taille-1 do
    for j = 0 to taille-1 do

      if i = etat.i && j = etat.j then () else
        Printf.printf "%2d" etat.grille.(i).(j)
    done;
  done;


type direction = Haut | Bas | Gauche | Droite | Rien

let delta = function
  | Haut -> (-1, 0)
  | Bas -> (1, 0)
  | Gauche -> (0, -1)
  | Droite -> (0, 1)
  | Rien -> assert false

let string_of_direction = function
  | Haut -> "Haut"
  | Bas -> "Bas"
  | Gauche -> "Gauche"
  | Droite -> "Droite"
  | Rien -> "Aucun mouvement"



(* Graphe du Taquin *)

let mouvements_possibles etat =
  let out = ref [] in
  let i = etat.i in
  let j = etat.j in
  let taille = Array.length etat.grille in

  if i > 0 then out := Haut::!out;
  if j > 0 then out := Gauche::!out;
  if i < taille-1 then out := Bas::!out;
  if j < taille-1 then out := Droite::!out;

  !out


let distance i j value =
  let i_target = value / n in
  let j_target = value mod n in
  abs (i - i_target) + abs (j - j_target)

let calcule_h etat =
  let n = Array.length etat.grille in
  let out = ref 0 in
  
  for i = 0 to n-1 do
    for j = 0 to n-1 do
      if i = etat.i && j = etat.j then () else
      out := !out + distance i j etat.grille.(i).(j)
    done
  done;

  etat.h <- !out

let add_pos posA posB =
  let i, j = posA in
  let ip, jp = posB in
  (i + ip, j + jp)


let delta_h etat move =
  (*
  On admet que le mouvement est possible

  On calcule la différence de :
    nouvelle position - ancienne position
  *)
  let i = etat.i in
  let j = etat.j in

  let ip, jp = add_pos (i, j) (delta move) in

  let nouvelle_dist = distance ip jp etat.grille.(ip).(jp) in

  let ancienne_dist = distance i j etat.grille.(ip).(jp) in

  ancienne_dist - nouvelle_dist

let applique etat move =
  if move = Rien then () else

  let i = etat.i in
  let j = etat.j in

  let ip, jp = add_pos (i, j) (delta move) in

  let swap = etat.grille.(i).(j) in

  etat.h <- etat.h + (delta_h etat move);

  etat.grille.(i).(j) <- etat.grille.(ip).(jp);
  etat.grille.(ip).(jp) <- swap;
  etat.i <- ip;
  etat.j <- jp


let copie etat =
  let taille = Array.length etat.grille in

  {
    grille = Array.init taille (fun i -> Array.copy etat.grille.(i));
    i = etat.i;
    j = etat.j;
    h = etat.h
  }



(* Quelques exemples pour les tests *)

(* état cible *)
let final =
  let m = Array.make_matrix n n 0 in
  for i = 0 to n - 1 do
    for j = 0 to n - 1 do
      m.(i).(j) <- i * n + j
    done
  done;
  {grille = m; i = n - 1; j = n - 1; h = 0}


(* Génération d'état par une suite aléatoire de nb_moves mouvements, *)
(* en partant de l'état final. Renvoi un état s tel que *)
(*  d(initial, s) <= nb_moves (de manière évidente). *)
let random_etat nb_moves =
  let etat = copie final in
  for i = 0 to nb_moves - 1 do
    let moves = mouvements_possibles etat in
    let n = List.length moves in
    applique etat (List.nth moves (Random.int n))
  done;
  etat



(*Décommenter ce qui suit quand les fonctions nécessaires ont été codées !*)

(* distance 10 *)
let dix =
  let moves = [Haut; Haut; Gauche; Gauche; Haut; Droite; Bas; Bas; Gauche; Gauche] in
  let etat = copie final in
  List.iter (applique etat) moves;
  etat



(* distance 20 *)
let vingt =
  {grille =
    [| [|0; 1; 2; 3|];
      [|12; 4; 5; 6|];
      [|8; 4; 10; 11|];
      [|13; 14; 7; 9|] |];
   i = 1; j = 1; h = 14}

(* distance 30 *)
let trente =
  {grille =
     [| [|8; 0; 3; 1|];
       [|8; 5; 2; 13|];
       [|6; 4; 11; 7|];
       [|12; 10; 9; 14|] |];
   i = 0; j = 0; h = 22}

(* distance 40 *)
let quarante =
  {grille =
     [| [|7; 6; 0; 10|];
       [|1; 12; 11; 3|];
       [|8; 4; 2; 5|];
       [|8; 9; 13; 14|] |];
   i = 2; j = 0; h = 30}

(* distance 50 *)
let cinquante =
  let s =
    {grille =
       [| [| 2; 3; 1; 6 |];
          [| 14; 5; 8; 4 |];
          [| 15; 12; 7; 9 |];
          [| 10; 13; 11; 0|] |];
     i = 2;
     j = 3;
     h = 0} in
  calcule_h s;
  s

(* distance 64 *)
let soixante_quatre =
  let s =
    {grille =
       [| [| 15; 14; 11; 7|];
          [| 5; 9; 12; 4|];
          [| 3; 10; 13; 8|];
          [| 2; 6; 0; 1|] |];
     i = 0;
     j = 0;
     h = 0} in
  calcule_h s;
  s

(* Part 2 *)


let successeurs etat =
  List.map (fun move ->
    let etatp = copie etat in
    applique etatp move;
    etatp
  )
  (mouvements_possibles etat)


let reconstruit (parents: ('a, 'a) Hashtbl.t) (x: 'a) =
  let out = ref [x] in

  while Hashtbl.find parents (List.hd !out) <> (List.hd !out) do
    out := Hashtbl.find parents (List.hd !out)::!out
  done;

  !out

exception Aucun_chemin


let egal_etats (etat1: etat) (etat2: etat) =
  let taille = Array.length etat1.grille in
  let out = ref true in
  
  if etat1.i <> etat2.i || etat1.j <> etat2.j then begin
    false

  end else begin
    let i, j = ref 0, ref 0 in
    while !out && !i < taille do
      j := 0;
      while !out && !j < taille do
        if !i <> etat1.i && !j <> etat1.j && etat1.grille.(!i).(!j) <> etat2.grille.(!i).(!j) then begin
          out := false
        end;
        j:=!j+1
      done;
      i:=!i+1
    done;

    !out
  end

let astar initial =
  (*
    Globalement, on suit les étapes du pseudo-code du cours
    1 - On initialise les variables, avec notamment Heap et Hashtbl
    2 - On continue tant que ouverts!=0 et qu'on a pas trouvé de sortie (=solution)
    3 - Si on a trouvé une sortie, on la retourne, sinon on lève une erreur.
  *)

  (*
    On stocke les distances sous forme d'une table de hashage
    Hashtbl.find_opt cle = None    ssi    dist[cle] = infiniiiiiiii
  *)
  let dist = Hashtbl.create 100 in
  Hashtbl.add dist initial 0;

  (*
    De même, on stocke les parents sous forme d'une table de hashage
  *)
  let parents = Hashtbl.create 100 in

  (*
    On utilise le module Heap (files de priorité) donné
  *)
  let ouverts = Heap.create () in

  (*
    On calcule au début le h (pour s'assurer que ça a été fait)
  *)
  calcule_h initial;

  (*
    On insère la source avec comme priorité son h
    (cette phrase est bizarre mais je saurais pas dire exactement dans quel contexte)
  *)
  Heap.insert ouverts (initial, initial.h);

  (* Pour pouvoir sortir de la boucle *)
  let sortie = ref true in

  
  while !sortie &&  Heap.length ouverts > 0 do
    (*
    taille de ouverts > 0 (par condition de la boucle while),
    donc on peut extraire un élément avec Option.get en étant safe.
    *)
    let premier = Heap.length ouverts in
    let u, dist_u = Option.get (Heap.extract_min ouverts) in
    let second = Heap.length ouverts in

    assert (premier = second + 1);

    Printf.printf "Taille de la file : %d\n" second;
    
    if egal_etats u final then begin
      (*
      On a trouvé la sortie, donc on sort de la boucle,
      et pour ça on passe sortie (condition de boucle) à false
      *)
      sortie := false
    end else begin

      List.iter (fun move ->

        let v = copie u in
        applique v move;
        
        let d = dist_u + 1 in


        (*
        Si la distance v n'a pas encore été trouvé (= cas du None),
        alors on le remplace par "l'infini", en pratique un entier très grand 
        *)
        let dist_v = match Hashtbl.find_opt dist v with
          | Some valeur -> valeur
          | None -> -1
        in

        if dist_v = (-1) || d < dist_v then begin
          Hashtbl.add parents v u;
          Hashtbl.add dist v d;
          
          if Heap.mem ouverts v then
            Heap.decrease_priority ouverts (v, (d + dist_v))
          else
            Heap.insert ouverts (v, (d + dist_v))
        end
      ) (mouvements_possibles u);
    end
  done;


  if not (!sortie) then
    (*
      On est sorti de la boucle avant sa fin,
      donc on a trouvé une solution. On la reconstruit avec la fonction reconstruit.
    *)
    reconstruit parents final
  else
    (*
      On est arrivé à la fin de la boucle sans trouvé de solution, :sob:
    *)
    []



(* Algorithme IDA* *)

exception Found of int

let idastar_length initial =
  failwith "TODO"


let idastar initial =
  failwith "TODO"

let print_direction_vector t =
  for i = 0 to Vector.length t - 1 do
    Printf.printf "%s " (string_of_direction (Vector.get t i))
  done;
  print_newline ()

let print_idastar etat =
  match idastar etat with
  | None -> print_endline "No path"
  | Some t ->
    Printf.printf "Length %d\n" (Vector.length t);
    print_direction_vector t


(*
let main () =
  Printexc.record_backtrace true

let () = main ()
*)