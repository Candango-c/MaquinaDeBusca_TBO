all: trab2

trab2: main.c
	gcc -o trab2 main.c


clean:
	rm -f *.o trab2


run: trab2
	./trab2