all: exec

libs: c-lib

c-lib: task1b.c
        # flags meanings:
        # -g = debug symbols
        # -m32 = compile to x86_32 architecture (32bit)
        # -c = Don't link (i.e., create an object file, not an executable)
        # -o numbers.o = output compiled file to numbers.o
	gcc -g -m32 -c -o task1b.o task1b.c

exec: libs task1b.o
	gcc -g -m32 task1b.o -o task1b
	rm task1b.o

.PHONY: clean
clean:
	rm -rf ./*.o main

