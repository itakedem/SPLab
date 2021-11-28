
%.o: %.asm
	nasm -g -O1 -f elf -o $@ $<

all: task2a task2b

task2a: task2a.o
	ld -melf_i386 -o task2a task2a.o

task2b: task2b.o cmpstr.o utoa_s.o 
	ld -melf_i386 -o task2b task2b.o

clean:
	rm -f *.o 
