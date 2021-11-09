all: main

main: main.o funcA.o
	gcc -m32 -o main main.o funcA.o

main.o: main.c
	gcc -m32 -c -o main.o main.c
	
funcA.o: funcA.asm
	nasm -g -f elf -w+all -o funcA.o funcA.asm

clean:
	rm -rf *.o main
