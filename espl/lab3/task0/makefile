all: task0b bubblesort

task0b: task0b.o
	gcc -m32 -g -Wall -o task0b task0b.o
	
task0b.o: task0b.c
	gcc -g -Wall -m32 -std=c99 -ansi -c -o task0b.o task0b.c

bubblesort: bubblesort.o
	gcc -m32 -g -Wall -o bubblesort bubblesort.o

bubblesort.o: bubblesort.c
	gcc -g -Wall -m32 -ansi -c -o bubblesort.o bubblesort.c


.PHONY: clean

clean: 
	rm -f *.o  bubblesort task0b
