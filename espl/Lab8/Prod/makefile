all: task1 task2 task3 patch

task1: task1.o
	gcc -m32 -g -Wall -o task1 task1.o

task1.o: task1.c
	gcc -g -Wall -m32 -ansi -c -o task1.o task1.c


task2: task2.o
	gcc -m32 -g -Wall -o task2 task2.o
	
task2.o: task2.c
	gcc -g -Wall -m32 -ansi -c -o task2.o task2.c


task3: task3.c
	gcc -g -m32 -fno-pie -c -o task3.o task3.c
	gcc -g -m32 task3.o -o task3

patch: patch.c
	gcc -g -m32 -fno-pie -c -o patch.o patch.c
	gcc -g -m32 patch.o -o patch


.PHONY: clean

clean: 
	rm -f *.o  task1 task2 task3
