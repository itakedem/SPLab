all: task1

task1: task1.o
	gcc -m32 -g -Wall -o task1 task1.o
	
task1.o: task1.c
	gcc -g -Wall -m32 -ansi -c -o task1.o task1.c
	
	
.PHONY: clean

clean: 
	rm -f *.o  task1
