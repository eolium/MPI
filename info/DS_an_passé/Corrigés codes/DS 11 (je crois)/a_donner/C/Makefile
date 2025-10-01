.PHONY : all, clean

all :
	gcc -c graph.c -o graph.o -Wall -Wextra -fsanitize=undefined
	gcc -c heap.c -o heap.o -Wall -Wextra -fsanitize=undefined
	gcc graph.o heap.o corrige.o capacite.c -o exec -Wall -Wextra -fsanitize=undefined -Wno-unused-parameter -Wno-return-type
	./exec
	
clean :
	rm heap.o graph.o exec
