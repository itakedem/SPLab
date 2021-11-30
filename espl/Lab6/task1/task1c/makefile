all: exec

exec: myshell.o LineParser.o
	gcc  -m32 -g -Wall -o exec myshell.o LineParser.o

myshell.o: myshell.c LineParser.o
	gcc -g -Wall -m32 -ansi -c -o myshell.o myshell.c

LineParser.o: LineParser.c
	gcc -g -Wall -m32 -ansi -c -o LineParser.o LineParser.c

#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean:
	rm -rf *.o exec