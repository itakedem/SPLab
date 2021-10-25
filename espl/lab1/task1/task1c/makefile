all: exec

libs: c-lib

c-lib: task1c.c
        # flags meanings:
        # -g = debug symbols
        # -m32 = compile to x86_32 architecture (32bit)
        # -c = Don't link (i.e., create an object file, not an executable)
        # -o numbers.o = output compiled file to numbers.o
	gcc -g -m32 -c -o task1c.o task1c.c

exec: libs task1c.o
	gcc -g -m32 task1c.o -o task1c
	rm task1c.o

.PHONY: clean
clean:
	rm -rf ./*.o main

