all: trab2

trab2: RBtree.o indexador.o main.c
	gcc -o trab2 RBtree.o indexador.o main.c

RBtree.o: RBtree.c RBtree.h
	gcc -c RBtree.c 

indexador.o: indexador.c indexador.h
	gcc -c indexador.c

clean:
	rm -f *.o trab2


run: trab2
	./trab2