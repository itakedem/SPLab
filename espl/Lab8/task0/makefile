all: examine

examine: examine.o
	gcc -m32 -g -Wall -o examine examine.o
	
examine.o: examine.c
	gcc -g -Wall -m32 -ansi -c -o examine.o examine.c
	
	
.PHONY: clean

clean: 
	rm -f *.o  examine
