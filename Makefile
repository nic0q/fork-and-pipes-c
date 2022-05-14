all: lab1 vis

funciones.o: funciones.c funciones.h
	gcc -c funciones.c -lm

lab1: lab1.c funciones.o
	gcc -o lab1 lab1.c funciones.o -lm

vis: vis.c funciones.o
	gcc -o vis vis.c funciones.o -lm

clean:
	rm -f *.o lab1 vis

run:	# Test
	./lab1 -i test100.csv -o propiedades.txt -n 4 -d 100 -b