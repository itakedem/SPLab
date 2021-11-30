all: prog

prog: myshell.o
	gcc  -m32 -g -Wall -o prog myshell.o

myshell.o:
	gcc -g -Wall -m32 -ansi -c -o myshell.o myshell.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean:
	rm -rf *.o prog