type litteral =
  | P of int
  | N of int

type clause = litteral * litteral 
type deuxcnf = clause list
type valuation = bool array


let eval_litt (litt: litteral) (valuation: valuation) =
  match litt with
  | P i -> valuation.(i)
  | N i -> not valuation.(i)


let rec eval (deuxcnf: deuxcnf) (valuation: valuation) =
  match deuxcnf with
  | [] -> failwith "eval sur une cnf vide !"
  | [cnf] ->
    let lit1, lit2 = cnf in
    (eval_litt lit1 valuation) && (eval_litt lit2 valuation)
  | cnf::reste ->
    let lit1, lit2 = cnf in
    ((eval_litt lit1 valuation) && (eval_litt lit2 valuation)) || eval reste valuation


exception Last

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


let brute_force deuxcnf =
  let valuation = Array.make 2 false in

  

  while 