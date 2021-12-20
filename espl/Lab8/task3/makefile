all: task3 patch fibonacci

task3: task3.o 
	gcc -g -m32 -Wall -o task3 task3.o 

task3.o: task3.c
	gcc -g -m32 -Wall -c -o task3.o task3.c

patch: patch.o 
	gcc -g -m32 -Wall -o patch patch.o 

patch.o: patch.c
	gcc -g -m32 -Wall -c -o patch.o patch.c

fibonacci: fibonacci.o 
	gcc -g -m32 -Wall -o fibonacci fibonacci.o 

fibonacci.o: fibonacci.c
	gcc -g -m32 -Wall -c -o fibonacci.o fibonacci.c


.PHONY: clean

clean:
	rm -f *.o task3 patch fibonacci