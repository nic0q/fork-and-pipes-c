all: lab1

lab1: lab1.c funciones.o
	gcc -o lab1 lab1.c funciones.o -lm

vis: vis.c funciones.o
	gcc -o vis vis.c funciones.o -lm

funciones.o: funciones.c funciones.h
	gcc -c funciones.c -lm

clean:
	rm -f *.o lab1 vis
