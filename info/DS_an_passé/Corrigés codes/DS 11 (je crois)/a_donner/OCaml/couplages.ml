open Corrige

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




let rec est_dans_couplage ar c = 
  failwith "not implemented"


let difference_symetrique c c' = 
  failwith "not implemented"

let rec est_couvert s c =
  failwith "not implemented"


let graphe_de_couplage gb c =
  failwith "not implemented"


let arbre_parcours g s = 
  failwith "not implemented"


let chemin g s t = 
  failwith "not implemented"


let couplage_maximum_biparti gb =
  failwith "not implemented"



let main () = Printf.printf "\nFichier couplages.ml compilé et exécuté avec succès.\n"
let () = main ()