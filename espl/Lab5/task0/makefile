# flags
C_COMPILE_FLAGS = -g -Wall -m32 -ansi -c
ASM_COMPILE_FLAGS = -g -f elf -w+all
LINK_FLAGS = -m32 -g -Wall

# All Target
all: prog

prog: task0.o asmIO.o
	gcc  -m32 -g -Wall -o prog task0.o asmIO.o

task0.o:
	gcc -g -Wall -m32 -ansi -c -o task0.o task0.c

asmIO.o: asmIO.asm
	nasm -g -f elf -w+all -o asmIO.o asmIO.asm


#tell make that "clean" is not a file name!
.PHONY: clean

#Clean the build directory
clean:
	rm -rf *.o prog