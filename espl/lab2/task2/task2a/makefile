all: task2a

task2a: task2a.o
	gcc -m32 -g -Wall -o task2a task2a.o

task2a.o: task2a.c
	gcc -g -Wall -m32 -ansi -c -o task2a.o task2a.c


.PHONY: clean

clean:
	rm -f *.o  task2a
