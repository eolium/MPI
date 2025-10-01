.PHONY: all, clean

all:
	ocamlc corrige.mli corrige.cmo couplages.ml -o exec
	./exec

clean : 
	rm couplages.cmi couplages.cmo exec
