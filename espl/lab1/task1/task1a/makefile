all: exec

libs: c-lib

c-lib: task1a.c
        # flags meanings:
        # -g = debug symbols
        # -m32 = compile to x86_32 architecture (32bit)
        # -c = Don't link (i.e., create an object file, not an executable)
        # -o numbers.o = output compiled file to numbers.o
	gcc -g -m32 -c -o task1a.o task1a.c

exec: libs my_echo.o
	gcc -g -m32 task1a.o -o task1a
	rm task1a.o

.PHONY: clean
clean:
	rm -rf ./*.o main

