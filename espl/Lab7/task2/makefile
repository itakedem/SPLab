all: exec

exec: task2.o
	gcc  -m32 -g -Wall -o exec task2.o

task2.o: task2.c
	gcc -g -Wall -m32 -ansi -c -o task2.o task2.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean:
	rm -rf *.o exec