let read_graph () =
  let n, p = Scanf.scanf "%d %d\n" (fun x y -> (x, y)) in
  let g = Array.make n [] in
  for i = 0 to p - 1 do
    Scanf.scanf "%d %d\n" (fun x y -> g.(x) <- y :: g.(x))
  done;
  g

type sommet = int
type graphe = sommet list array


let transpose x =
  let t = Array.make (Array.length x) [] in
  
  for i = 0 to Array.length x -1 do
    List.iter (fun v -> begin t.(v) <- i::(t.(v)) end) x.(i);
  done;
  t


let rec dfs_rec x s0 vus visite =
  (*
    - Si s0 n'a pas été visité :
    - Pour chacun de ses voisins : {
      - dfs_rec x s0 visite
      }
    - On l'éxec après pour faire un postfixe
    - On l'ajoute à vus
  *)
  if (not vus.(s0)) then begin
    vus.(s0) <- true;

    List.iter (fun v -> dfs_rec x v vus visite) x.(s0);

    visite := s0::(!visite)
  end else
    ()


let dfs_post x =
  let vus = Array.make (Array.length x) false in
  let visite = ref [] in

  for s0 = 0 to Array.length x -1 do
    dfs_rec x s0 vus visite
  done;
  !visite


let kosaraju g =
  let l = dfs_post g in
  let composantes = ref [] in
  let vus = Array.make (Array.length g) false in
  let miroir = transpose g in

  let rec ajouter x c =
    if not vus.(x) then begin
      vus.(x) <- true;
      List.iter (fun y ->
        ajouter y c
      ) miroir.(x);
      c := x::!c
    end
  in

  List.iter (fun x ->
    if not vus.(x) then begin
      let c = ref [] in
      ajouter x c;
      composantes := !c :: !composantes
    end
  ) l;

  List.rev !composantes



let main () =
  let g = read_graph () in
  let t = transpose g in
  print_string "--- Test de lecture du graphe. ---\nLe sommet d'indice 0 du graphe a les voisins suivants :\n";
  List.iter (Printf.printf "%d ") t.(0);
  print_newline ();
  let parcours = dfs_post g in
  List.iter (fun s -> Printf.printf "%d " s) parcours;
  print_newline ();
  let cfc = kosaraju g in
  Printf.printf "Nb composantes fortement connexes : %d\n" (List.length cfc);
  List.iteri (fun i comp ->
    Printf.printf "CFC #%d : " (i+1);
    List.iter (fun s -> Printf.printf "%d " s) comp;
    print_newline();
    ) cfc


let prog () =
  let g = read_graph () in
  let cfc = kosaraju g in

  Printf.printf "Nb composantes fortement connexes : %d\n" (List.length cfc);
  List.iteri (fun i comp ->
    Printf.printf "CFC #%d : " (i+1);
    List.iter (fun s -> Printf.printf "%d " s) comp;
    print_newline();
    ) cfc

(*
let () = prog ()
*)



type litteral = P of int | N of int
type clause = litteral * litteral
type deuxcnf = clause list
type valuation = bool array

exception Last


let eval_litt (litt: litteral) (valuation: valuation) =
  match litt with
  | P i -> valuation.(i)
  | N i -> not valuation.(i)


let rec eval (deuxcnf: deuxcnf) (valuation: valuation) =
  match deuxcnf with
  | [] -> true
  | clause::reste ->
    let lit1, lit2 = clause in
    ((eval_litt lit1 valuation) || (eval_litt lit2 valuation)) && eval reste valuation


let incremente_valuation valuation =
  let rec recur n =
    if n = (-1) then
      raise Last
    else if valuation.(n) then begin
      valuation.(n) <- false;
      recur (n-1)
    end else
      valuation.(n) <- true
  in

  recur (Array.length valuation -1)


let print_array tab = Array.iter (Printf.printf "%b ") tab; Printf.printf "\n"

let brute_force (deuxcnf: deuxcnf) =
  let taille = List.length deuxcnf in
  let valuation = Array.make taille false in

  let rec recur () =
    try
      if eval deuxcnf valuation then
        Some valuation
      else begin
      incremente_valuation valuation;
      recur()
      end
    with
    | Last -> None
  in
  
  recur()

let satisfiable x = failwith "not implemented"
let temoin x = failwith "not implemented"



let graphe_de_cnf (cnf: deuxcnf) =
  let len = List.length cnf in
  let g = Array.make (2*len) [] in

  let node_of_litt litt =
    match litt with
    | P(a) -> 2*a
    | N(a) -> 2*a+1
  in

  let node_of_not_litt litt =
    match litt with
    | P(a) -> 2*a+1
    | N(a) -> 2*a
  in

  List.iter (fun (clause: clause) ->
    let l1, l2 = clause in

    g.(node_of_not_litt l1) <- (node_of_litt l2)::g.(node_of_not_litt l1);
    g.(node_of_not_litt l2) <- (node_of_litt l1)::g.(node_of_not_litt l2);
    ) cnf;

  g


let exemple_f = [(P 0, N 2); (P 1, P 3); (N 1, P 2); (N 2, P 3); (P 3, N 0)]

let print_graphe g = 
  for i = 0 to Array.length g -1 do
    Printf.printf "%d -> " i;    
    List.iter (fun v -> Printf.printf "%d " v) g.(i);
    Printf.printf "\n"
  done


let satisfiable deuxcnf =
  let g = graphe_de_cnf deuxcnf in
  let cfc = kosaraju g in
  let taille = Array.length g in
  let marquage = Array.make taille (-1) in

  List.iteri (fun i composante -> begin
    List.iter (fun s -> begin
      if marquage.(s) == -1 then
        marquage.(s) <- i
      else
        failwith "What ?!"
      end
    ) composante;
    end
  ) cfc;

  let out = ref true in
  for i = 0 to (taille / 2) do
    out := !out && (marquage.(i) <> marquage.(i+1)) 
  done;

  !out

(*let temoin deuxcnf =

  let g = graphe_de_cnf deuxcnf in
  let cfc = kosaraju g in
  let taille = Array.length g in
  let marquage = Array.make taille (-1) in

    let valuation = Array.make taille false in

    (*
      - Pour chaque CFC {
        - Si not(s0) a déjà été set, on let set en opposé
        - On met tous les autres à not s0
      }
    *)

    let inverse n =
      let moitie = 

    List.iteri (fun i composante -> begin
      if marquage.() = -1 then
        valuation.
        ;
      List.iter (fun s -> begin
        if marquage.(s) == -1 then
          marquage.(s) <- i
        else
          failwith "What ?!"
        end
      ) composante;
      end
  ) cfc;

  let out = ref true in
  for i = 0 to (taille / 2) do
    out := !out && (marquage.(i) <> marquage.(i+1)) 
  done;

  if not !out then None else begin

    List.iteri (fun i composante -> begin
      if  
    List.iter (fun s -> begin
      if marquage.(s) == -1 then
        marquage.(s) <- i
      else
        failwith "What ?!"
      end
    ) composante;
    end
  ) cfc;


    ;None
  end
*)

let test_erreur = [(P 0, N 1); (N 0, P 1)]

let _ = Printf.printf "%b\n" (satisfiable exemple_f)