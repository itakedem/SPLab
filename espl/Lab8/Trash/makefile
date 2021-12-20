all: task3 patch fibonacci

task3: task3.c
	gcc -g -m32 -fno-pie -c -o task3.o task3.c
	gcc -g -m32 task3.o -o task3

fibonacci: fibonacci.c
	gcc -g -m32 -fno-pie -c -o fibonacci.o fibonacci.c
	gcc -g -m32 fibonacci.o -o fibonacci

patch: patch.c
	gcc -g -m32 -fno-pie -c -o patch.o patch.c
	gcc -g -m32 patch.o -o patch

.PHONY: clean
clean:
	rm -rf ./*.o patch task3 fibonacci
