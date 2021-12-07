all: exec

exec: mypipeline.o
	gcc  -m32 -g -Wall -o exec mypipeline.o

mypipeline.o: mypipeline.c
	gcc -g -Wall -m32 -ansi -c -o mypipeline.o mypipeline.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean:
	rm -rf *.o exec