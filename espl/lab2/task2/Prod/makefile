all: task2a task2b task2c task3

task2a: task2a.o
	gcc -m32 -g -Wall -o task2a task2a.o

task2b: task2b.o
	gcc -m32 -g -Wall -o task2b task2b.o

task2c: task2c.o
	gcc -m32 -g -Wall -o task2c task2c.o

task3: task3.o
	gcc -m32 -g -Wall -o task3 task3.o

task2a.o: task2a.c
	gcc -g -Wall -m32 -ansi -c -o task2a.o task2a.c

task2b.o: task2b.c
	gcc -g -Wall -m32 -ansi -c -o task2b.o task2b.c

task2c.o: task2c.c
	gcc -g -Wall -m32 -ansi -c -o task2c.o task2c.c

task3.o: task3.c
	gcc -g -Wall -m32 -ansi -c -o task3.o task3.c

.PHONY: clean
clean:
	rm -rf ./*.o task2a task2b task2c task3

