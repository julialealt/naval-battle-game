all:	main

main:	main.o funcoes.o
	gcc main.o funcoes.o -o main

main.o:	main.c funcoes.h
	gcc -g -c main.c

funcoes.o:	funcoes.c funcoes.h
	gcc -g -c funcoes.c

clean:
	rm -f *.o main

