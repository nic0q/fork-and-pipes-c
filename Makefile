all: lab1

lab1: lab1.c funciones.o
	gcc -o lab1 lab1.c funciones.o

vis: vis.c funciones.o
	gcc -o vis vis.c funciones.o

funciones.o: funciones.c funciones.h
	gcc -c funciones.c

clean:
	rm -f *.o lab1 vis
