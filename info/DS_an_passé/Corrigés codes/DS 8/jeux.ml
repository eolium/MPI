let transpose g =
  let n = Array.length g in
  let gt = Array.make n [] in
  for s = 0 to n - 1 do
      List.iter (fun t -> gt.(t) <- s :: gt.(t)) g.(s)
  done;
  gt

let strategie g tab =
  let n = Array.length g in
  let gt = transpose g in
  let f = Array.make n (-1) in
  let rec dfs s t =
  if f.(t) < 0 then begin
  f.(t) <- if tab.(t) then n else s;
  List.iter (dfs t) gt.(t)
  end in
  for s = 0 to n - 1 do
  if tab.(s) then dfs n s
  done;
  f

let attracteur g s1 tab =
  let n = Array.length g in
  let gt = transpose g and
  file = Queue.create () and
  attr = Array.make n false in
  let deg_attr = Array.make n 0 and
  degre = Array.init n (fun s -> List.length g.(s)) in
  let ajout_sommet s =
  Queue.push s file;
  attr.(s) <- true;
  List.iter (fun t -> deg_attr.(t) <- deg_attr.(t) + 1) gt.(s) in
  let traiter t =
  if (not attr.(t)) &&
  ((s1.(t) && deg_attr.(t) > 0) ||
  (not s1.(t) && degre.(t) = deg_attr.(t))) then
  ajout_sommet t in
  for s = 0 to n - 1 do
  if tab.(s) then ajout_sommet s
  done;
  while not (Queue.is_empty file) do
  let s = Queue.pop file in
  List.iter traiter gt.(s)
  done;
  attr
