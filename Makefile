all: vis lab1

vis: vis.o vis.c
	gcc -o vis vis.c

vis: lab1.o lab1.c
	gcc -o lab1 lab1.c

lab1.o: lab1.c lab1.h
	gcc -c lab1.c

vis.o: vis.c vis.h
	gcc -c vis.c