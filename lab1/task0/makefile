all: exec

libs: c-lib

c-lib: my_echo.c
        # flags meanings:
        # -g = debug symbols
        # -m32 = compile to x86_32 architecture (32bit)
        # -c = Don't link (i.e., create an object file, not an executable)
        # -o numbers.o = output compiled file to numbers.o
	gcc -g -m32 -c -o my_echo.o my_echo.c

exec: libs my_echo.o
	gcc -g -m32 my_echo.o -o my_echo
	rm my_echo.o

.PHONY: clean
clean:
	rm -rf ./*.o main

