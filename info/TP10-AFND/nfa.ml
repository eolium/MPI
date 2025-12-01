type 'a regex =
  | Empty
  | Eps
  | Letter of 'a
  | Union of 'a regex * 'a regex
  | Concat of 'a regex * 'a regex
  | Star of 'a regex


(* ----- Affichages de regex ----- *)

(* Parses a string into an int regex.
 * The alphabet is assumed to be a subset of a..z, and is converted
 * to [0..25] (a -> 0, b -> 1...),
 * Charcater '&' stands for "epsilon", and character '#' for "empty".
 * Spaces are ignored, and the usual priority rules apply.
 *)

let parse string =
  let open Printf in
  let to_int c =
    assert ('a' <= c && c <= 'z');
    int_of_char c - int_of_char 'a' in
  let s = Stream.of_string string in
  let rec peek () =
    match Stream.peek s with
    | Some ' ' -> Stream.junk s; peek ()
    | Some c -> Some c
    | None -> None in
  let eat x =
    match peek () with
    | Some y when y = x -> Stream.junk s; ()
    | Some y -> failwith (sprintf "expected %c, got %c" x y)
    | None -> failwith "incomplete" in
  let rec regex () =
    let t = term () in
    match peek () with
    | Some '|' -> eat '|'; Union (t, regex ())
    | _ -> t
  and term () =
    let f = factor () in
    match peek () with
    | None | Some ')' | Some '|' -> f
    | _ -> Concat (f, term ())
 and factor () =
    let rec aux acc =
      match peek () with
      | Some '*' -> eat '*'; aux (Star acc)
      | _ -> acc in
    aux (base ())
  and base () =
    match peek () with
    | Some '(' -> eat '('; let r = regex () in eat ')'; r
    | Some '&' -> eat '&'; Eps
    | Some '#' -> eat '#'; Empty
    | Some (')' | '|' | '*' as c) -> failwith (sprintf "unexpected '%c'" c)
    | Some c -> eat c; Letter (to_int c)
    | None -> failwith "unexpected end of string" in
  let r = regex () in
  try Stream.empty s; r
  with _ -> failwith "trailing ')' ?"


let rec string_of_regex e =
  let open Printf in
  let to_char i =
    char_of_int (i + int_of_char 'a') in
  let priorite = function
    | Union (_, _) -> 1
    | Concat (_, _) -> 2
    | Star _ -> 3
    | _ -> 4 in
  let parenthese expr parent =
    if priorite expr < priorite parent then
      sprintf "(%s)" (string_of_regex expr)
    else string_of_regex expr in
  match e with
  | Empty -> "#"
  | Eps -> "&"
  | Letter x -> sprintf "%c" (to_char x)
  | Union (f, f') -> sprintf "%s|%s" (parenthese f e) (parenthese f' e)
  | Concat (f, f') -> sprintf "%s%s" (parenthese f e) (parenthese f' e)
  | Star f -> sprintf "%s*" (parenthese f e)




(* ----- Affichage d'automates (graphviz) ----- *)

type state = int

type nfa =
  {delta : state list array array;
  accepting : bool array}

let graphviz_nfa a filename =
  let open Printf in
  let n = Array.length a.delta in
  let m = Array.length a.delta.(0) in
  let out = open_out filename in
  fprintf out "digraph a {\nrankdir = LR;\n";
  (* noms des états *)
  let lettre i = String.make 1 (char_of_int (i + int_of_char 'a')) in
  (* etats *)
  for q = 0 to n - 1 do
    let shape = if a.accepting.(q) then "doublecircle" else "circle" in
    fprintf out "node [shape = %s, label = %d] %d;\n" shape q q
  done;
  (* etat initial *)
  fprintf out "node [shape = point]; I\n";
  fprintf out "I -> %i;\n" 0;
  (* transitions *)
    let labels = Array.make_matrix n n [] in
  for q = 0 to n - 1 do
    for x = m - 1 downto 0 do
      let ajoute q' = labels.(q).(q') <- lettre x :: labels.(q).(q') in
      List.iter ajoute a.delta.(q).(x)
    done
  done;
  for q = 0 to n - 1 do
    for q' = 0 to n - 1 do
      let s = String.concat "," labels.(q).(q') in
      if s <> "" then
        fprintf out "%i -> %i [ label = \"%s\" ];\n" q q' s
    done
  done;
  fprintf out "}\n";
  close_out out

let genere_pdf input_file output_file =
  Sys.command (Printf.sprintf "dot -Tpdf %s -o %s" input_file output_file)


type dfa =
  {delta_d : state array array;
  accepting_d : bool array}

let to_nfa a =
  let n = Array.length a.delta_d in
  let m = Array.length a.delta_d.(0) in
  let delta = Array.make_matrix n m [] in
  for q = 0 to n - 1 do
    for x = 0 to m - 1 do
      delta.(q).(x) <- [a.delta_d.(q).(x)]
    done
  done;
  {delta = delta; accepting = a.accepting_d}

let graphviz_dfa a = graphviz_nfa (to_nfa a)





(* ----- Exemples de regex à utiliser ----- *)


let e1 = parse "(a|b)#" (* langage vide *)
let e2 = parse "(b|ca)*d" 
let e3 = parse "&|a(b|ca)*"
let e4 = parse "baba"

(* exemple d'affichage de regex :*)
let () = print_string "\n----- affichage des regex :----\ne1 = ";
  print_string (string_of_regex e1); 
  print_string "\ne2 = ";
  print_string (string_of_regex e2); 
  print_string "\ne3 = ";
  print_string (string_of_regex e3); 
  print_newline ()

let exemple =
  Concat (Union (Letter 'c', Letter 'a'),
          Star (Concat (Letter 'b',
                        Union (Letter 'a', Letter 'c'))))
  
  





(* ----- A vous de jouer ! ----- *)



let rec merge (u: 'a list) (v: 'a list) =
  match u, v with
  | u, [] -> u
  | [], v -> v
  | t1::q1, t2::q2 ->
    if t1 = t2 then merge q1 v else
    if t1 < t2 then
      t1::(merge q1 v)
    else
      t2::(merge u q2)



let rec is_empty (r: 'a regex) = 
  match r with
  | Empty -> true
  | Eps -> false
  | Letter(l) -> false
  | Union(r1, r2) -> is_empty r1 && is_empty r2
  | Concat(r1, r2) -> is_empty r1 || is_empty r2
  | Star(r1) -> false


let rec contains_epsilon r =
  match r with
  | Empty -> false
  | Eps -> true
  | Letter(l) -> false
  | Union(r1, r2) -> contains_epsilon r1 || contains_epsilon r2
  | Concat(r1, r2) -> contains_epsilon r1 && contains_epsilon r2
  | Star(r1) -> true


let rec prefix r = 
  if is_empty r then [] else

  match r with
  | Empty -> []
  | Eps -> []
  | Letter(a) -> [a]
  | Union(u, v) -> merge (prefix u) (prefix v)
  | Concat(u, v) ->
    if contains_epsilon u then
      merge (prefix u) (prefix v)
    else
      prefix u
  | Star(u) -> prefix u


let rec suffix r =
  if is_empty r then [] else

  match r with
  | Empty -> []
  | Eps -> []
  | Letter(a) -> [a]
  | Union(u, v) -> merge (suffix u) (suffix v)
  | Concat(u, v) ->
    if contains_epsilon v then
      merge (suffix u) (suffix v)
    else
      suffix v
  | Star(u) -> suffix u


let combine u v =
  let out = ref [] in

  List.iter (fun e ->
    List.iter (fun d ->
      out := (d, e)::(!out)
    ) u
  ) v;

  !out



let rec factor (r: 'a regex) : ('a * 'a) list =
  if is_empty r then [] else

  match r with
  | Empty -> []
  | Eps -> []
  | Letter(a) -> []
  | Concat(Letter(a), Letter(b)) -> [(a, b)]
  | Union(u, v) -> merge (factor u) (factor v)
  | Concat(u, v) -> merge
    (merge (factor u) (factor v))
    (combine (suffix u) (prefix v)) 
  | Star u -> merge
    (factor u)
    (combine (suffix u) (prefix u))


let rec number_of_letters r =
  match r with
  | Empty -> 0
  | Eps -> 0
  | Letter(_) -> 1
  | Union(u, v) -> (number_of_letters u) + (number_of_letters v)
  | Concat(u, v) -> (number_of_letters u) + (number_of_letters v)
  | Star(u) -> number_of_letters(u)



let linearize x = failwith "not implemented"
let max_letter x = failwith "not implemented"
let glushkov x = failwith "not implemented"
let delta_set x = failwith "not implemented"

let has_accepting_state x = failwith "not implemented"
let nfa_accept x = failwith "not implemented"
let build_set x = failwith "not implemented"
let powerset x = failwith "not implemented"









