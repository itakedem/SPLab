all: exec

exec: myPipe.o
	gcc  -m32 -g -Wall -o exec myPipe.o

myPipe.o: myPipe.c
	gcc -g -Wall -m32 -ansi -c -o myPipe.o myPipe.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean:
	rm -rf *.o exec