all: trab2

trab2: RBtree.o indexador.o pageRanking.o processadorCon.o main.c
	gcc -o trab2 RBtree.o indexador.o pageRanking.o processadorCon.o main.c -lm

RBtree.o: RBtree.c RBtree.h
	gcc -c RBtree.c 

indexador.o: indexador.c indexador.h
	gcc -c indexador.c

pageRanking.o: pageRanking.c pageRanking.h
	gcc -c pageRanking.c

processadorCon.o: processadorCon.c processadorCon.h
	gcc -c processadorCon.c

clean:
	rm -f *.o trab2


run: trab2
	./trab2