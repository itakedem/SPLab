all: exec

task1a: task1a.o
	gcc -m32 -g -Wall -o task1a task1a.o

task1b: task1b.o
	gcc -m32 -g -Wall -o task1b task1b.o

task1c: task1c.o
	gcc -m32 -g -Wall -o task1c task1c.o

task2: task2.o
	gcc -m32 -g -Wall -o task2 task2.o

task1a.o: task1a.c
	gcc -g -Wall -m32 -ansi -c -o task1a.o task1a.c

task1b.o: task1b.c
	gcc -g -Wall -m32 -ansi -c -o task1b.o task1b.c

task1c.o: task1c.c
	gcc -g -Wall -m32 -ansi -c -o task1c.o task1c.c

task2.o: task2.c
	gcc -g -Wall -m32 -ansi -c -o task2.o task2.c

.PHONY: clean
clean:
	rm -rf ./*.o exec

