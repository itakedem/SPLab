all: task2Full

task2Full: task2Full.o
	gcc -m32 -g -Wall -o task2Full task2Full.o

task2Full.o: task2Full.c
	gcc -g -Wall -m32 -ansi -c -o task2Full.o task2Full.c


.PHONY: clean

clean:
	rm -f *.o  task2Full
