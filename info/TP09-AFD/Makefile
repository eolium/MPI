.PHONY : all, clean

all :
	gcc -c afdio.c -o afdio.o -Wall -Wextra -fsanitize=undefined
	gcc afdio.o afd.c -o test -Wall -Wextra -fsanitize=undefined
	./test
	
clean :
	rm *.o test

